#ifndef CENTER_H
#define CENTER_H

#include <stddef.h>

namespace Center
{
extern const double S2;
extern const double DELTA_X[];
extern const double DELTA_Y[];

/**
 * @struct
 * @brief  Options for how the geometric center should be calculated
 *
 * @prop   epsilon   acceptable margin of error
 * @prop   bounds    a multiplier of the range of points to search
 * @prop   subsearch whether to search obliquely
 */
struct GeometricCenterOptions {
  const double epsilon;
  const double bounds;
  const bool   subsearch;
};

/**
 * @brief   Calculates the net cost of travelling from a set of points to their
 *          center
 * @details Uses Pythagorean distance for cost measurement
 *
 * @param   x         center x coordinate
 * @param   y         center y coordinate
 * @param   points    points to measure distance from
 * @param   numPoints number of points
 *
 * @return  net cost of travelling to the center
 */
double cost(double x, double y, const double points[][2], size_t numPoints);

/**
 * @brief   Calculates the net cost of travelling from a set of points to their
 *          center
 * @details Uses Manhattan distance for cost measurement
 * @note    Effective mostly only for taxi-cab-like problems
 *
 * @param   x         center x coordinate
 * @param   y         center y coordinate
 * @param   points    points to measure distance from
 * @param   numPoints number of points
 *
 * @return  net cost of travelling to the center
 */
double manhattanCost(double       x,
                     double       y,
                     const double points[][2],
                     size_t       numPoints);

/**
 * @brief   Finds the center of a set of points
 * @details Assumes all points have equal weight. Puts the center of mass in a
 *          user-designated array.
 *
 * @param   points    points to measure
 * @param   numPoints number of points
 * @param   fill      array to fill with center of mass
 */
void centerOfMass(const double points[][2], size_t numPoints, double fill[2]);

/**
 * @brief   Finds the geometric center of a set of points.
 * @details Fills an array with the geometric center of an arbitrary amount of
 *          points. Returns the score (total cost to center) of the geometric
 *          center.
 *          The algorithm is a simple Newtonian search. We iterate an
 *          indiscriminate amount of times through smaller bounds until we
 *          approve some margin of error. Note that local maxima are a
 *          non-issue, as the geometric median is (unique and covergent for
 *          non-co-linear
 *          points)[http://www.stat.rutgers.edu/home/cunhui/papers/39.pdf].
 *
 * @param   points    points to find the center of
 * @param   numPoints number of points
 * @param   options   specified margin of error, bound range, and subsearch
 *                    value
 * @param   fill      array to fill with geometric center
 *
 * @return  geometric center of a set of points
 */
double geometricCenter(const double                   points[][2],
                       size_t                         numPoints,
                       const GeometricCenterOptions & options,
                       double                         fill[2]);
}  // namespace Center

#endif
