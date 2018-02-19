#ifndef CARTESIAN_H
#define CARTESIAN_H

#include <cmath>

namespace Cartesian
{
extern const long double PI;
extern const size_t      EARTH_RADIUS_METERS;
extern const double      METER_TO_KM;
extern const long double METER_TO_MI;

double radiansFromDeg(double degrees);
double haversine(double startLat,
                 double endLat,
                 double distLat,
                 double distLng,
                 char   unit);

}  // namespace Cartesian

#endif
