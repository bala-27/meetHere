#include "tsp.hpp"

/**
 * TODO: Figure out node object wrapping so `cost` doesn't need to be duplicated
 */
namespace TSP {

/**
 * Finds the point nearest the current node (city) with Branch and Bound on the
 * cost matrix.
 */
short branchBound(double **matrix, const unsigned int &len,
                  const unsigned int &city, const bool *visited) {
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
 * A std::any_of for dynamic arrays
 */
template <typename T>
bool contains(const T *arr, const unsigned int &len, const T &value) {
  for (unsigned int i = 0; i < len; ++i) {
    if (arr[i] == value) {
      return true;
    }
  }
  return false;
}

/**
 * Summates the Pythagorean distance between a center and an arbitrary amount
 * of points as an efficient and powerful cost function.
 */
double cost(double **points, const unsigned char &length, const double &x,
            const double &y) {
  double cost = 0;

  for (unsigned int i = 0; i < length; ++i) {
    cost += std::sqrt(std::pow((points[i][0] - x), 2) +
                      std::pow((points[i][1] - y), 2));
  }
  return cost;
}

/**
 * Summates the Manhattan distance between a center and an arbitrary amount
 * of points as an efficient and powerful cost function for VRP.
 *
 * NOTE Effictive mostly only in Manhattan-style planes.
 */
double manhattanCost(double **points, const unsigned char &length,
                     const double &x, const double &y) {
  double cost = 0;

  for (unsigned int i = 0; i < length; ++i) {
    cost += std::abs(points[i][0] - x) + std::abs(points[i][1] - y);
  }
  return cost;
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
  const char method = (char)(args[2]->Uint32Value());

  const unsigned int len = _points->Length();

  // setup visited-cities and cost matrix
  // std::vector<bool> visited(len, false);
  bool *visited = new bool[len]();

  double **matrix = new double *[len];
  for (unsigned int i = 0; i < len; ++i) {
    matrix[i] = new double[len];
  }

  const unsigned char matrixLen = 1;

  // fill cost matrix
  for (unsigned int i = 0; i < len; ++i) {
    for (unsigned int j = 0; j < len; ++j) {
      double **to = new double *[2];
      to[0] = new double[2];
      double from[2];
      for (unsigned char k = 0; k < 2; ++k) {
        to[0][k] =
            v8::Local<v8::Array>::Cast(_points->Get(j))->Get(k)->NumberValue();
        from[k] =
            v8::Local<v8::Array>::Cast(_points->Get(i))->Get(k)->NumberValue();
      }

      if (method == 't') {
        matrix[i][j] = cost(to, matrixLen, from[0], from[1]);
      } else if (method == 'n') {
        matrix[i][j] = manhattanCost(to, matrixLen, from[0], from[1]);
      }
    }
  }

  // setup order matrix
  v8::Local<v8::Array> order = v8::Array::New(isolate);
  order->Set(0, v8::Number::New(isolate, city));

  // calculate nearest node (city) and add it to order matrix until every node
  // has been visited
  while (contains(visited, len, false)) {
    visited[city] = true;
    const short nearest = branchBound(matrix, len, city, visited);
    if (nearest == -1) {
      break;
    }
    order->Set(order->Length(), v8::Number::New(isolate, nearest));
    city = nearest;
  }

  // free memory
  for (unsigned int i = 0; i < len; ++i) {
    delete[] matrix[i];
  }
  delete[] matrix;

  args.GetReturnValue().Set(order);
} // namespace TSP

void init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "tsp", tsp);
}

NODE_MODULE(addon, init);

} // namespace TSP
