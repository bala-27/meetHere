#include "center.hpp"

namespace Center {

/**
 * Finds the center of mass of an arbitrary amount of points using standard
 * median.
 */
std::array<double, 2> centerOfMass(const double points[][2],
                                   const unsigned int &length) {
  double x = 0;
  double y = 0;

  for (unsigned int i = 0; i < length; ++i) {
    x += points[i][0];
    y += points[i][1];
  }

  return {{x / length, y / length}};
}

/**
 * Summates the Pythagorean distance between a center and an arbitrary amount
 * of points as an efficient and powerful cost function.
 */
double cost(const double points[][2], const unsigned int &length,
            const double &x, const double &y) {
  double cost = 0;

  for (unsigned int i = 0; i < length; ++i) {
    cost += std::sqrt(std::pow((points[i][0] - x), 2) +
                      std::pow((points[i][1] - y), 2));
  }
  return cost;
}

/**
 * Calculates the geometric center of an arbitrary amount of points.
 *
 * This is done through a simple Newtonian search. We iterate an indiscriminate
 * amount of times through smaller bounds until we approve some margin of error
 * (epsilon). Note that local maxima is a non-issue, as the geometric median is
 * (unique and covergent for non-co-linear
 * points)[http://www.stat.rutgers.edu/home/cunhui/papers/39.pdf].
 */
void geometric(const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::Isolate *isolate = args.GetIsolate();

  // get args
  v8::Local<v8::Array> _points = v8::Local<v8::Array>::Cast(args[0]);
  bool subsearch = args[1]->BooleanValue();
  double epsilon = args[2]->NumberValue();
  double bounds = args[3]->NumberValue();

  const unsigned int length = _points->Length();

  // pass locations to C++ array
  double points[length][2];
  for (unsigned int i = 0; i < length; ++i) {
    v8::Local<v8::Array> element = v8::Local<v8::Array>::Cast(_points->Get(i));
    points[i][0] = element->Get(0)->NumberValue();
    points[i][1] = element->Get(1)->NumberValue();
  }

  // setup initial values
  std::array<double, 2> center = centerOfMass(points, length);
  double score = cost(points, length, center[0], center[1]);
  double step = score / length * bounds;

  // search
  while (step > epsilon) {
    bool improved = false;
    for (int i = 0; i < 8; subsearch ? ++i : (i += 2)) {
      const double _x = center[0] + step * delta_x[i];
      const double _y = center[1] + step * delta_y[i];
      const double _score = cost(points, length, _x, _y);

      if (_score < score) {
        center[0] = _x;
        center[1] = _y;
        score = _score;
        improved = !improved;
        break;
      }
    }

    if (!improved) {
      step /= 2;
    }
  }

  // convert center back to JS Array
  v8::Local<v8::Array> _center = v8::Array::New(isolate);
  _center->Set(0, v8::Number::New(isolate, center[0]));
  _center->Set(1, v8::Number::New(isolate, center[1]));

  // create object to hold center and score
  v8::Local<v8::Object> result = v8::Object::New(isolate);
  result->Set(v8::String::NewFromUtf8(isolate, "center"), _center);
  result->Set(v8::String::NewFromUtf8(isolate, "score"),
              v8::Number::New(isolate, score));

  args.GetReturnValue().Set(result);
}

void init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "geometric", geometric);
}

NODE_MODULE(addon, init);

} // namespace Center
