#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <cmath>
#include "util.h"

namespace Polynomial
{
extern const size_t DEFAULT_DEGREE;

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
size_t guessPolynomialDegree(const double x[], const double y[], size_t length);

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
void fillCoefficientsFromNormalMatrix(Util::DoubleArr2D matrix,
                                      double            fill[],
                                      size_t            length);

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
void fillNormalMatrix(const double *    sigmaX,
                      const double *    sigmaY,
                      Util::DoubleArr2D fill,
                      size_t            polyDegree);

/**
 * @brief Calculates the sigma values of x coordinates.
 *
 * @param xPos         x positions
 * @param numXPos      number of x positions
 * @param sigmaX       array to fill with sigma values
 * @param lengthSigmaX number of sigma values
 */
void fillSigmaX(const double xPos[],
                size_t       numXPos,
                double       sigmaX[],
                size_t       lengthSigmaX);

/**
 * @brief Calculates the sigma values of y coordinates.
 *
 * @param yPos         y positions
 * @param numYPos      number of y positions
 * @param sigmaY       array to fill with sigma values
 * @param lengthSigmaY number of sigma values
 */
void fillSigmaY(const double xPos[],
                const double yPos[],
                size_t       numPos,
                double       sigmaY[],
                size_t       lengthSigmaY);

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
void fillBestFit(const double xPos[],
                 const double yPos[],
                 size_t       numPoints,
                 size_t       polynomialDegree,
                 double       fill[]);

}  // namespace Polynomial

#endif
