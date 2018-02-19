#include "tsp.h"
#include "util.h"
#include <limits>

/**
 * Finds the node nearest the current node (city) on the cost matrix using
 * Branch and Bound.
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
