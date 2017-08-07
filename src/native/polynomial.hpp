#include <cmath>
#include <node.h>

namespace Polynomial {

const unsigned char DEFAULT_DEGREE = 2;

extern double coefficientsFromMatrix(double *, unsigned char &);
extern double guessPolynomialDegree(double *, double *, unsigned int &);
extern double normalGuassianMatrix(double, double, unsigned char &);
extern double sigmaX(double *, unsigned char &, unsigned int &);
extern double sigmaY(double *, double *, unsigned char &, unsigned int &);

} // namespace Polynomial
