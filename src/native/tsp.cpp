#include "tsp.h"
#include "util.h"
#include <limits>

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
int TSP::nearestCity(const Util::DoubleArr2D costMatrix,
                     size_t                  len,
                     size_t                  currentCity,
                     const bool              visited[len])
{
  int    nearest = -1;
  size_t minCost = std::numeric_limits<size_t>::max();

  for (size_t newCity = 0; newCity < len; ++newCity) {
    const size_t _minCost = costMatrix[currentCity][newCity];

    if (_minCost != 0 && !visited[newCity]) {
      if (_minCost < minCost) {
        minCost = _minCost;
        nearest = newCity;
      }
    }
  }

  return nearest;
}
