#include "../polynomial.h"
#include "../util.h"
#include <cmath>
#include <node.h>

namespace Polynomial
{
/**
 * Calculates best-fit polynomial function of some degree based on multiple
 * provided points.
 */
void bestFit(const v8::FunctionCallbackInfo<v8::Value> & args)
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

  const size_t lengthSigmaX = 2 * degree + 1;
  const size_t lengthSigmaY = degree + 1;
  double       sigmaX[lengthSigmaX];
  double       sigmaY[lengthSigmaY];

  fillSigmaX(xPos, numPoints, sigmaX, lengthSigmaX);
  fillSigmaY(xPos, yPos, numPoints, sigmaY, lengthSigmaY);

  // normal matrix of shape [degree + 1][degree + 2]
  Util::DoubleArr2D normalMatrix = new Util::DoubleArr[degree + 2];
  for (size_t i = 0; i < degree + 1; ++i) {
    normalMatrix[i] = new double[degree + 2];
  }
  fillNormalMatrix(sigmaX, sigmaY, normalMatrix, degree);

  const size_t gaussianDegree = degree + 1;
  double       coeffs[gaussianDegree];
  fillCoefficientsFromNormalMatrix(normalMatrix, coeffs, gaussianDegree);

  // pass coeffs back to JS Array
  v8::Local<v8::Array> _coeffs = v8::Array::New(isolate);
  for (unsigned char i = 0; i < degree + 1; ++i) {
    _coeffs->Set(i, v8::Number::New(isolate, coeffs[i]));
  }

  // free memory
  for (size_t i = 0; i < degree + 1; ++i) {
    delete[] normalMatrix[i];
  }
  delete[] normalMatrix;

  args.GetReturnValue().Set(_coeffs);
}

void init(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "bestFit", bestFit);
}

NODE_MODULE(addon, init);

}  // namespace Polynomial
