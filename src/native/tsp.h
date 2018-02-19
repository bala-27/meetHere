#ifndef TSP_H
#define TSP_H

#include "util.h"
#include <cmath>

namespace TSP
{
/**
 * @enum
 * @brief The method for traversing points for an efficient route
 *
 * @prop  tsp      Travelling Salesman Problem
 * @prop  naiveVsp Vehicle Routing Problem, with Manhattan distance
 */
enum VisitMethod { tsp = 't', naiveVrp = 'n' };

/**
 * @brief   Calculates the nearest unvisited city to a specified one
 * @details Iterates over a cost matrix to find the cheapest, unvisited city
 *          to travel to from the current one.
 *
 * @param   costMatrix  costs of travelling between each city
 * @param   len         size of the cost matrix
 * @param   currentCity city to find the next one from
 * @param   visited     tracks visited cities
 *
 * @return  index of nearest city, or -1 if no cities left
 */
int nearestCity(const Util::DoubleArr2D costMatrix,
                size_t                  len,
                size_t                  currentCity,
                const bool              visited[len]);

}  // namespace TSP

#endif
