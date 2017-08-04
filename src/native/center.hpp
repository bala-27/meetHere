#include <cmath>
#include <node.h>

namespace Center {

/*
 *           (0,1)
 *    (-S2,S2)   (S2,S2)
 * (-1,0)      x       (1,0)
 *    (-S2,-S2)  (S2,-S2)
 *          (0,-1)
 */
const double S2 = std::sqrt(2) / 2;
const double DELTA_X[] = {-1, -S2, 0, S2, 1, S2, 0, -S2};
const double DELTA_Y[] = {0, S2, 1, S2, 0, -S2, -1, -S2};

extern double centerOfMass(double[][2], unsigned int &);
extern double cost(double[][2], unsigned int &, double &, double &);

} // namespace Center
