#include <array>
#include <cmath>
#include <node.h>

namespace Center {

/*
 *           (0,1)
 *    (-s2,s2)   (s2,s2)
 * (-1,0)      x       (1,0)
 *    (-s2,-s2)  (s2,-s2)
 *          (0,-1)
 */
const double s2 = std::sqrt(2) / 2;
const double delta_x[] = {-1, -s2, 0, s2, 1, s2, 0, -s2};
const double delta_y[] = {0, s2, 1, s2, 0, -s2, -1, -s2};

extern std::array<double, 2> centerOfMass(double[][2], unsigned int &);
extern double cost(double[][2], unsigned int &, double &, double &);

} // namespace Center
