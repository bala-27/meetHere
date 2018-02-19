#include "../polynomial.h"
#include "../util.h"
#include <node.h>

namespace Polynomial
{
/**
 * Calculates the best-fit polynomial function of an arbitrary set of points.
 */
void wrapBestFit(const v8::FunctionCallbackInfo<v8::Value> & args)
{
  v8::Isolate * isolate = args.GetIsolate();

  v8::Local<v8::Array> _points   = v8::Local<v8::Array>::Cast(args[0]);
  const size_t         numPoints = _points->Length();
  double               xPos[numPoints];
  double               yPos[numPoints];
  v8::Local<v8::Array> orig;

  for (size_t i = 0; i < numPoints; ++i) {
    orig    = v8::Local<v8::Array>::Cast(_points->Get(i));
    xPos[i] = orig->Get(0)->NumberValue();
    yPos[i] = orig->Get(1)->NumberValue();
  }

  size_t degree = args[1]->Uint32Value();
  if (!degree) {
    degree = guessPolynomialDegree(xPos, yPos, numPoints);
  }

  // calculate polynomial
  double coeffs[degree + 1];
  fillBestFit(xPos, yPos, numPoints, degree, coeffs);

  // pass coeffs back to JS Array
  v8::Local<v8::Array> _coeffs = v8::Array::New(isolate);
  for (unsigned char i = 0; i < degree + 1; ++i) {
    _coeffs->Set(i, v8::Number::New(isolate, coeffs[i]));
  }

  args.GetReturnValue().Set(_coeffs);
}

void init(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "bestFit", wrapBestFit);
}

NODE_MODULE(addon, init);

}  // namespace Polynomial
