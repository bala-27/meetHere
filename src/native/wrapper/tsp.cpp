#include "../center.h"
#include "../tsp.h"
#include "../util.h"
#include <node.h>

namespace TSP
{
/**
 * Determines the shortest-travel path between planar points using Branch and
 * Bound. The operation records nodes (cities) travelled to and continues until
 * there are no more cities left to travel to.
 */
void calTSP(const v8::FunctionCallbackInfo<v8::Value> & args)
{
  v8::Isolate * isolate = args.GetIsolate();

  // get args
  v8::Local<v8::Array> _points   = v8::Local<v8::Array>::Cast(args[0]);
  const size_t         numPoints = _points->Length();
  const size_t         startCity = args[1]->Uint32Value();
  const char           method    = (char)(args[2]->Uint32Value());

  // setup visited-cities
  bool visited[numPoints];

  // setup cost matrix
  Util::DoubleArr2D costMatrix = new Util::DoubleArr[numPoints];
  for (size_t i = 0; i < numPoints; ++i) {
    *(costMatrix + i) = new double[numPoints];
  }

  const size_t matrixLen = 1;

  // fill cost matrix
  for (size_t i = 0; i < numPoints; ++i) {
    for (size_t j = 0; j < numPoints; ++j) {
      double to[matrixLen][2];
      double from[2];
      for (size_t k = 0; k < 2; ++k) {
        to[0][k] =
            v8::Local<v8::Array>::Cast(_points->Get(j))->Get(k)->NumberValue();
        from[k] =
            v8::Local<v8::Array>::Cast(_points->Get(i))->Get(k)->NumberValue();
      }

      switch (method) {
        case VisitMethod::tsp:
          costMatrix[i][j] = Center::cost(from[0], from[1], to, matrixLen);
        case VisitMethod::naiveVsp:
          costMatrix[i][j] =
              Center::manhattanCost(from[0], from[1], to, matrixLen);
      }
    }
  }

  // setup order matrix
  size_t               city  = startCity;
  v8::Local<v8::Array> order = v8::Array::New(isolate);
  order->Set(0, v8::Number::New(isolate, city));

  // calculate nearest node (city) and add it to order matrix until every node
  // has been visited
  while (Util::arr_contains(visited, numPoints, false)) {
    visited[city]     = true;
    const int nearest = nearestCity(costMatrix, numPoints, city, visited);
    if (nearest == -1) {
      break;
    }
    order->Set(order->Length(), v8::Number::New(isolate, nearest));
    city = nearest;
  }

  args.GetReturnValue().Set(order);
}  // namespace TSP

void init(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "tsp", calTSP);
}

NODE_MODULE(addon, init);

}  // namespace Center
