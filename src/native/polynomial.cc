#include "polynomial.hpp"
#include <algorithm>

namespace Polynomial {

/**
 * Determines polynomial coefficients from normal matrix.
 */
double *coefficientsFromMatrix(double **matrix, const unsigned char &degree) {
  const unsigned char gaussianDegree = degree + 1;
  double *coefficients = new double[gaussianDegree];

  // backwards substitution
  for (int i = gaussianDegree - 1; i >= 0; --i) {
    // variable is dependent on RHS
    coefficients[i] = matrix[i][gaussianDegree];
    for (int j = 0; j < gaussianDegree; ++j) {
      // if we are not on an equal identity, remove all but direct coefficient
      if (j != i) {
        coefficients[i] -= matrix[i][j] * coefficients[j];
      }
    }
    // divide by coeff of variable to calculate
    coefficients[i] /= matrix[i][i];
  }

  return coefficients;
}

/**
 * Attempts to guess degree of polynomial to use, for optimal efficiency.
 */
char guessPolynomialDegree(const double *x, double *y,
                           const unsigned int &length) {
  if (length < 2) {
    return 0;
  }

  unsigned char extrema = 0;
  bool increasing;
  double *sortedY = y;
  {
    bool swapped = true;
    unsigned char j = 0;
    while (swapped) {
      swapped = !swapped;
      j++;
      for (unsigned char i = 0; i < length - j; ++i) {
        if (x[i] > x[i + 1]) {
          std::swap(sortedY[i], sortedY[i + 1]);
          swapped = true;
        }
      }
    }
  }

  increasing = sortedY[0] < sortedY[1];
  for (unsigned char i = 1; i < length - 1; ++i) {
    const bool nowIncr = sortedY[i] < sortedY[i + 1];
    if (increasing != nowIncr) {
      extrema++;
      increasing = nowIncr;
    }
  }

  return DEFAULT_DEGREE + extrema;
}

/**
 * Returns an augmented normal matrix with coordinate sigma summations, with
 * additional guassian elimination supported by a pivot point.
 */
double **normalGuassianMatrix(const double *sigmaX, const double *sigmaY,
                              const unsigned char &degree) {
  // [degree + 1][degree + 2]
  double **normalMatrix = new double *[degree + 2];
  for (unsigned char i = 0; i < degree + 1; ++i) {
    normalMatrix[i] = new double[degree + 2];
  }

  // store coefficients in normal matrix
  for (unsigned char i = 0; i <= degree; ++i) {
    for (unsigned char j = 0; j <= degree; ++j) {
      normalMatrix[i][j] = sigmaX[i + j];
    }
    normalMatrix[i][degree + 1] = sigmaY[i];
  }

  // Perform
  const unsigned char gaussianDegree = degree + 1;

  // set up guassian elimination + eliminate elements under pivot point
  for (unsigned char i = 0; i < gaussianDegree; ++i) {
    for (unsigned char k = i + 1; k < gaussianDegree; ++k) {
      // setup
      if (normalMatrix[i][i] < normalMatrix[k][i]) {
        for (unsigned char j = 0; j <= gaussianDegree; ++j) {
          std::swap(normalMatrix[i][j], normalMatrix[k][j]);
        }
      }

      // elimination
      const double t = normalMatrix[k][i] / normalMatrix[i][i];
      for (unsigned char j = 0; j <= gaussianDegree; ++j) {
        normalMatrix[k][j] -= t * normalMatrix[i][j];
      }
    }
  }

  return normalMatrix;
}

/**
 * Return x-coor sigma summation values.
 */
double *sigmaX(const double *x, const unsigned char &degree,
               const unsigned int &length) {
  const unsigned int sigmaXLength = 2 * degree + 1;
  double *sigmaX = new double[sigmaXLength]();
  for (unsigned int i = 0; i < sigmaXLength; ++i) {
    for (unsigned int j = 0; j < length; ++j) {
      sigmaX[i] += std::pow(x[j], i);
    }
  }

  return sigmaX;
}

/**
 * Return y-coor sigma summation values.
 */
double *sigmaY(const double *x, const double *y, const unsigned char &degree,
               const unsigned int &length) {
  const unsigned int sigmaYLength = degree + 1;
  double *sigmaY = new double[sigmaYLength]();
  for (unsigned int i = 0; i < sigmaYLength; ++i) {
    for (unsigned int j = 0; j < length; ++j) {
      sigmaY[i] += std::pow(x[j], i) * y[j];
    }
  }

  return sigmaY;
}

/**
 * Calculates best-fit polynomial function of some degree based on multiple
 * provided points.
 */
void bestFit(const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::Isolate *isolate = args.GetIsolate();

  v8::Local<v8::Array> _points = v8::Local<v8::Array>::Cast(args[0]);
  const unsigned int length = _points->Length();
  double _x[length];
  double _y[length];

  for (unsigned int i = 0; i < length; ++i) {
    v8::Local<v8::Array> _element = v8::Local<v8::Array>::Cast(_points->Get(i));
    _x[i] = _element->Get(0)->NumberValue();
    _y[i] = _element->Get(1)->NumberValue();
  }

  unsigned char degree = (unsigned char)((args[1]->Uint32Value()));
  if (!degree) {
    degree = guessPolynomialDegree(_x, _y, length);
  }

  double *x = sigmaX(_x, degree, length);
  double *y = sigmaY(_x, _y, degree, length);
  double **normalMatrix = normalGuassianMatrix(x, y, degree);
  const double *coeffs = coefficientsFromMatrix(normalMatrix, degree);

  // pass coeffs back to JS Array
  v8::Local<v8::Array> _coeffs = v8::Array::New(isolate);
  for (unsigned char i = 0; i < degree + 1; ++i) {
    _coeffs->Set(i, v8::Number::New(isolate, coeffs[i]));
  }

  args.GetReturnValue().Set(_coeffs);
}

void init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "bestFit", bestFit);
}

NODE_MODULE(addon, init);

} // namespace Polynomial
