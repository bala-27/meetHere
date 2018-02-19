#include "polynomial.h"
#include "util.h"
#include <algorithm>
#include <cmath>

const size_t Polynomial::DEFAULT_DEGREE = 2;

/**
 * @brief   Guesses the optimal degree of a polynomial function for a set of
 *          points.
 * @details Sorts the points in increasing function order, and then records the
 *          number of extrema observed while traversing the points.
 *
 * @param   x      x coordinates of the points
 * @param   y      y coordinates of the points
 * @param   length number of points
 *
 * @return  best degree of polynomial to approximate
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
 * @brief   Determines the coefficients of a polynomial function from its normal
 *          matrix.
 * @details Performs backsubstitution on a matrix in reduced row echelon form
 *          to calculate its solution, which corresponds to the coefficients of
 *          a polynomial function. The coefficients are then filled to an array.
 *
 * @param   matrix matrix to derive solution from
 * @param   fill   matrix to fill with polynomial coefficients
 * @param   length dimension of the solution matrix
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
 * @brief   Derives an augmented normal matrix from the summation of coordinate
 *          sigmas.
 * @details Saves coefficients to a normal matrix, then performs gaussian
 *          elimination to eliminate elements under the main diagonal pivot.
 *
 * @param   sigmaX     sigma values of x coordinates
 * @param   sigmaY     sigma values of y coordinates
 * @param   fill       augmented matrix to fill
 * @param   polyDegree degree of the polynomial function
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

  // set up gaussian elimination + eliminate elements under pivot
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
 * @brief Calculates the sigma values of x coordinates.
 *
 * @param xPos         x positions
 * @param numXPos      number of x positions
 * @param sigmaX       array to fill with sigma values
 * @param lengthSigmaX number of sigma values
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
 * @brief Calculates the sigma values of y coordinates.
 *
 * @param yPos         y positions
 * @param numYPos      number of y positions
 * @param sigmaY       array to fill with sigma values
 * @param lengthSigmaY number of sigma values
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

/**
 * @brief Calculates the best-fit polynomial function for a set of coordinate
 *        points.
 *
 * @param xPos             set of x coordinates
 * @param yPos             set of y coordinates
 * @param numPoints        number of coordinates
 * @param polynomialDegree degree of polynomial function to approximate
 * @param fill             array to fill with polynomial coefficients
 */
void Polynomial::fillBestFit(const double xPos[],
                             const double yPos[],
                             size_t       numPoints,
                             size_t       polynomialDegree,
                             double       fill[])
{
  const size_t lengthSigmaX = 2 * polynomialDegree + 1;
  const size_t lengthSigmaY = polynomialDegree + 1;

  double sigmaX[lengthSigmaX];
  for (size_t i = 0; i < lengthSigmaX; ++i) {
    sigmaX[i] = 0;
  }

  double sigmaY[lengthSigmaY];
  for (size_t i = 0; i < lengthSigmaY; ++i) {
    sigmaY[i] = 0;
  }

  fillSigmaX(xPos, numPoints, sigmaX, lengthSigmaX);
  fillSigmaY(xPos, yPos, numPoints, sigmaY, lengthSigmaY);

  // normal matrix of shape [degree + 1][degree + 2]
  Util::DoubleArr2D normalMatrix = new Util::DoubleArr[polynomialDegree + 2];
  for (size_t i = 0; i < polynomialDegree + 1; ++i) {
    normalMatrix[i] = new double[polynomialDegree + 2];
  }
  fillNormalMatrix(sigmaX, sigmaY, normalMatrix, polynomialDegree);

  const size_t gaussianDegree = polynomialDegree + 1;
  fillCoefficientsFromNormalMatrix(normalMatrix, fill, gaussianDegree);

  // free memory
  for (size_t i = 0; i < polynomialDegree + 1; ++i) {
    delete[] normalMatrix[i];
  }
  delete[] normalMatrix;
}
