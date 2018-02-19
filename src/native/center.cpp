#include "center.h"
#include <cmath>

/*
 *           (0,1)
 *    (-S2,S2)   (S2,S2)
 * (-1,0)      x       (1,0)
 *    (-S2,-S2)  (S2,-S2)
 *          (0,-1)
 */
const double Center::S2        = std::sqrt(2) / 2;
const double Center::DELTA_X[] = {-1, -S2, 0, S2, 1, S2, 0, -S2};
const double Center::DELTA_Y[] = {0, S2, 1, S2, 0, -S2, -1, -S2};

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
double Center::cost(double       x,
                    double       y,
                    const double points[][2],
                    size_t       numPoints)
{
  double cost = 0;

  for (size_t i = 0; i < numPoints; ++i) {
    cost += std::sqrt(std::pow((points[i][0] - x), 2) +
                      std::pow((points[i][1] - y), 2));
  }
  return cost;
}

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
double Center::manhattanCost(double       x,
                             double       y,
                             const double points[][2],
                             size_t       numPoints)
{
  double cost = 0;

  for (size_t i = 0; i < numPoints; ++i) {
    cost += std::abs(points[i][0] - x) + std::abs(points[i][1] - y);
  }
  return cost;
}

/**
 * @brief   Finds the center of a set of points
 * @details Assumes all points have equal weight. Puts the center of mass in a
 *          user-designated array.
 *
 * @param   points    points to measure
 * @param   numPoints number of points
 * @param   fill      array to fill with center of mass
 */
void Center::centerOfMass(const double points[][2],
                          size_t       numPoints,
                          double       fill[2])
{
  double center[2] = {0, 0};

  for (size_t i = 0; i < numPoints; ++i) {
    center[0] += points[i][0];
    center[1] += points[i][1];
  }

  fill[0] = center[0] / numPoints;
  fill[1] = center[1] / numPoints;
}

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
double Center::geometricCenter(const double                   points[][2],
                               size_t                         numPoints,
                               const GeometricCenterOptions & options,
                               double                         fill[2])
{
  // fill center to CoM, calculate initial score and step
  centerOfMass(points, numPoints, fill);
  double score = cost(fill[0], fill[1], points, numPoints);
  double step  = score / numPoints * options.bounds;

  // descend gradient, searching for the function minimum, until the error
  // reaches some acceptable epsilon.
  while (step > options.epsilon) {
    bool improved = false;

    // check points a step in each direction to find one of lower cost
    for (size_t i = 0; i < 8; options.subsearch ? ++i : (i += 2)) {
      const double _x     = fill[0] + step * DELTA_X[i];
      const double _y     = fill[1] + step * DELTA_Y[i];
      const double _score = cost(_x, _y, points, numPoints);

      if (_score < score) {
        fill[0] = _x, fill[1] = _y;
        score    = _score;
        improved = true;
        break;
      }
    }

    if (!improved) {  // no improvement means error can be improved
      step /= 2;
    }
  }

  return score;
}
