#include "polynomial.h"
#include "util.h"
#include <algorithm>
#include <cmath>

const size_t Polynomial::DEFAULT_DEGREE = 2;

/**
 * Attempts to guess degree of polynomial to use, for optimal efficiency.
 */
size_t Polynomial::guessPolynomialDegree(const double x[],
                                         const double y[],
                                         size_t       length)
{
  if (length < 2) {
    return 0;
  }

  double sortedY[length];
  for (size_t i = 0; i < length; ++i) {
    sortedY[i] = y[i];
  }

  {
    bool   swapped = true;
    size_t j       = 0;

    while (swapped) {
      swapped = false;
      ++j;
      for (size_t i = 0; i < length - j; ++i) {
        if (x[i] > x[i + 1]) {
          std::swap(sortedY[i], sortedY[i + 1]);
          swapped = true;
        }
      }
    }
  }

  bool   slope   = sortedY[0] < sortedY[1];
  size_t extrema = 0;
  for (size_t i = 1; i < length - 1; ++i) {
    const bool _slope = sortedY[i] < sortedY[i + 1];
    if (slope != _slope) {
      ++extrema;
      slope = _slope;
    }
  }

  return DEFAULT_DEGREE + extrema;
}

/**
 * Determines polynomial coefficients from normal matrix.
 */
void Polynomial::fillCoefficientsFromNormalMatrix(Util::DoubleArr2D matrix,
                                                  double            fill[],
                                                  size_t            length)
{
  // backwards substitution
  for (size_t i = length; i > 0; --i) {
    // variable is dependent on RHS
    fill[i - 1] = matrix[i - 1][length];
    for (size_t j = 0; j < length; ++j) {
      // if we are not on an equal identity, remove all but direct coefficient
      if (j != (i - 1)) {
        fill[i - 1] -= matrix[i - 1][j] * fill[j];
      }
    }
    // divide by coeff of variable to calculate
    fill[i - 1] /= matrix[i - 1][i - 1];
  }
}

/**
 * Returns an augmented normal matrix with coordinate sigma summations, with
 * additional guassian elimination supported by a pivot point.
 */
void Polynomial::fillNormalMatrix(const double *    sigmaX,
                                  const double *    sigmaY,
                                  Util::DoubleArr2D fill,
                                  size_t            polyDegree)
{
  // store coefficients in normal matrix
  for (size_t i = 0; i <= polyDegree; ++i) {
    for (size_t j = 0; j <= polyDegree; ++j) {
      fill[i][j] = sigmaX[i + j];
    }
    fill[i][polyDegree + 1] = sigmaY[i];
  }

  const size_t gaussianDegree = polyDegree + 1;

  // set up guassian elimination + eliminate elements under pivot
  for (size_t i = 0; i < gaussianDegree; ++i) {
    for (size_t j = i + 1; j < gaussianDegree; ++j) {
      // setup
      if (fill[i][i] < fill[j][i]) {
        for (size_t k = 0; k <= gaussianDegree; ++k) {
          std::swap(fill[i][k], fill[j][k]);
        }
      }

      // elimination
      const double t = fill[j][i] / fill[i][i];
      for (size_t k = 0; k <= gaussianDegree; ++k) {
        fill[j][k] -= t * fill[i][k];
      }
    }
  }
}

/**
 * Return x-coor sigma summation values.
 */
void Polynomial::fillSigmaX(const double xPos[],
                            size_t       numXPos,
                            double       sigmaX[],
                            size_t       lengthSigmaX)
{
  for (size_t i = 0; i < lengthSigmaX; ++i) {
    for (size_t j = 0; j < numXPos; ++j) {
      sigmaX[i] += std::pow(xPos[j], i);
    }
  }
}

/**
 * Return y-coor sigma summation values.
 */
void Polynomial::fillSigmaY(const double xPos[],
                            const double yPos[],
                            size_t       numPos,
                            double       sigmaY[],
                            size_t       lengthSigmaY)
{
  for (size_t i = 0; i < lengthSigmaY; ++i) {
    for (size_t j = 0; j < numPos; ++j) {
      sigmaY[i] += std::pow(xPos[j], i) * yPos[j];
    }
  }
}
