#include "../center.h"
#include <node.h>

namespace Center
{
/**
 * Calculates the geometric center of an arbitrary amount of points.
 *
 * This is done through a simple Newtonian search. We iterate an indiscriminate
 * amount of times through smaller bounds until we approve some margin of error
 * (epsilon). Note that local maxima is a non-issue, as the geometric median is
 * (unique and covergent for non-co-linear
 * points)[http://www.stat.rutgers.edu/home/cunhui/papers/39.pdf].
 */
void geometric(const v8::FunctionCallbackInfo<v8::Value> & args)
{
  v8::Isolate * isolate = args.GetIsolate();

  // get args
  v8::Local<v8::Array>         _points   = v8::Local<v8::Array>::Cast(args[0]);
  const size_t                 numPoints = _points->Length();
  const bool                   subsearch = args[1]->BooleanValue();
  const double                 epsilon   = args[2]->NumberValue();
  const double                 bounds    = args[3]->NumberValue();
  const GeometricCenterOptions opts      = {epsilon, bounds, subsearch};

  // pass locations to native array
  double points[numPoints][2];
  for (unsigned int i = 0; i < numPoints; ++i) {
    v8::Local<v8::Array> _element = v8::Local<v8::Array>::Cast(_points->Get(i));
    points[i][0]                  = _element->Get(0)->NumberValue();
    points[i][1]                  = _element->Get(1)->NumberValue();
  }

  // calculate geometric center
  double       center[2] = {0, 0};
  const double score     = geometricCenter(points, numPoints, opts, center);

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

/**
 * Wrapper for centerOfMass function
 */
void mass(const v8::FunctionCallbackInfo<v8::Value> & args)
{
  v8::Isolate * isolate = args.GetIsolate();

  // get args
  v8::Local<v8::Array> _points = v8::Local<v8::Array>::Cast(args[0]);

  const unsigned int length = _points->Length();

  // pass locations to C++ array
  double points[length][2];
  for (unsigned int i = 0; i < length; ++i) {
    v8::Local<v8::Array> element = v8::Local<v8::Array>::Cast(_points->Get(i));
    points[i][0]                 = element->Get(0)->NumberValue();
    points[i][1]                 = element->Get(1)->NumberValue();
  }

  // get results
  double center[2] = {0, 0};
  centerOfMass(points, length, center);
  const double score = cost(center[0], center[1], points, length);

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

void init(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "geometric", geometric);
  NODE_SET_METHOD(exports, "mass", mass);
}

NODE_MODULE(addon, init);

}  // namespace Center
