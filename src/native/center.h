#ifndef CENTER_H
#define CENTER_H

#include <cmath>

namespace Center
{
extern const double S2;
extern const double DELTA_X[];
extern const double DELTA_Y[];

struct GeometricCenterOptions {
  const double epsilon;
  const double bounds;
  const bool   subsearch;
};

double cost(double x, double y, const double points[][2], size_t numPoints);
double manhattanCost(double       x,
                     double       y,
                     const double points[][2],
                     size_t       numPoints);

void centerOfMass(const double points[][2], size_t numPoints, double fill[2]);
double geometricCenter(const double                   points[][2],
                       size_t                         numPoints,
                       const GeometricCenterOptions & options,
                       double                         fill[2]);
}  // namespace Center

#endif
