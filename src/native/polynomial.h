#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <cmath>
#include "util.h"

namespace Polynomial
{
extern const size_t DEFAULT_DEGREE;

size_t guessPolynomialDegree(const double x[], const double y[], size_t length);
void fillCoefficientsFromNormalMatrix(Util::DoubleArr2D matrix,
                                      double            fill[],
                                      size_t            length);
void fillNormalMatrix(const double *    sigmaX,
                      const double *    sigmaY,
                      Util::DoubleArr2D fill,
                      size_t            polyDegree);
void fillSigmaX(const double xPos[],
                size_t       numXPos,
                double       sigmaX[],
                size_t       lengthSigmaX);
void fillSigmaY(const double xPos[],
                const double yPos[],
                size_t       numPos,
                double       sigmaY[],
                size_t       lengthSigmaY);

}  // namespace Polynomial

#endif
