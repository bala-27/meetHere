#ifndef TSP_H
#define TSP_H

#include "util.h"
#include <cmath>

namespace TSP
{
enum VisitMethod { tsp = 't', naiveVsp = 'n' };

int nearestCity(const Util::DoubleArr2D costMatrix,
                size_t                  len,
                size_t                  currentCity,
                const bool              visited[len]);

}  // namespace TSP

#endif
