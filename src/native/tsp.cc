#include "tsp.hpp"

/**
 * TODO: Figure out node object wrapping so `cost` doesn't need to be duplicated
 */
namespace TSP {

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
 * Finds the point nearest the current node (city) with Branch and Bound on the
 * cost matrix.
 */
short branchBound(const std::vector<std::vector<double>> &matrix,
                  const unsigned int &len, const unsigned int &city,
                  std::vector<bool> &visited) {
  short nearest = -1;
  unsigned int min = std::numeric_limits<int>::max();
  for (unsigned int to = 0; to < len; ++to) {
    const unsigned int _min = matrix[city][to];
    if (_min != 0 && !visited[to]) {
      if (_min < min) {
        min = _min;
        nearest = to;
      }
    }
  }
  return nearest;
}

/**
 * Determines the shortest-travel path between planar points using Branch and
 * Bound. The operation records nodes (cities) travelled to and continues until
 * there are no more cities left to travel to.
 */
void tsp(const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::Isolate *isolate = args.GetIsolate();

  // get args
  v8::Local<v8::Array> _points = v8::Local<v8::Array>::Cast(args[0]);
  unsigned int city = args[1]->Uint32Value();
  const unsigned int len = _points->Length();

  // setup visited-cities and cost matrix
  std::vector<bool> visited(len, false);
  std::vector<std::vector<double>> matrix;
  matrix.resize(len);
  for (auto &i : matrix)
    i.resize(len);

  // fill cost matrix
  for (unsigned int i = 0; i < len; ++i) {
    for (unsigned int j = 0; j < len; ++j) {
      double to[1][2] = {
          {v8::Local<v8::Array>::Cast(_points->Get(j))->Get(0)->NumberValue(),
           v8::Local<v8::Array>::Cast(_points->Get(j))->Get(1)->NumberValue()}};

      double from[2] = {
          v8::Local<v8::Array>::Cast(_points->Get(i))->Get(0)->NumberValue(),
          v8::Local<v8::Array>::Cast(_points->Get(i))->Get(1)->NumberValue()};

      matrix[i][j] = cost(to, 1, from[0], from[1]);
    }
  }

  // setup order matrix
  v8::Local<v8::Array> order = v8::Array::New(isolate);
  order->Set(0, v8::Number::New(isolate, city));

  // calculate nearest node (city) and add it to order matrix until every node
  // has been visited
  while (std::any_of(visited.begin(), visited.end(),
                     [](const bool &p) { return p == false; })) {
    visited[city] = true;
    const short nearest = branchBound(matrix, len, city, visited);
    if (nearest == -1) {
      break;
    }
    order->Set(order->Length(), v8::Number::New(isolate, nearest));
    city = nearest;
  }

  args.GetReturnValue().Set(order);
}

void init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "tsp", tsp);
}

NODE_MODULE(addon, init);

} // namespace TSP
