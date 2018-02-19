#ifndef CARTESIAN_H
#define CARTESIAN_H

#include <stddef.h>

namespace Cartesian
{
extern const long double PI;
extern const size_t      EARTH_RADIUS_METERS;
extern const double      METER_TO_KM;
extern const long double METER_TO_MI;

/**
 * @brief  Returns the radian measurement of a degree value
 *
 * @param  degrees the degree measurement
 *
 * @return the radian equivalent of the degrees
 */
double radiansFromDeg(double degrees);

/**
 * @brief   Calculates the earthly distance between two cartesian points
 * @details Uses the Haversine formula to calculate the distance between two
 *          Latitude/Longitude points
 *          a = sin²(Δφ/2) + cos φ1 ⋅ cos φ2 ⋅ sin²(Δλ/2)
 *          c = 2 ⋅ atan2( √a, √(1−a) )
 *          d = R ⋅ c
 *
 * @param   startLat  starting latitude
 * @param   endLat    ending latitude
 * @param   distLat   distance between latitudes
 * @param   distLng   distance between longitudes
 * @param   unit      type of unit to use: meters or miles
 *
 * @return  distance between two cartesian points
 */
double haversine(double startLat,
                 double endLat,
                 double distLat,
                 double distLng,
                 char   unit);

}  // namespace Cartesian

#endif
