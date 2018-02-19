#include <cmath>
#include "cartesian.h"

const long double Cartesian::PI                  = std::acos(-1.0L);
const size_t      Cartesian::EARTH_RADIUS_METERS = 6371e3;
const double      Cartesian::METER_TO_KM         = 1e-3;
const long double Cartesian::METER_TO_MI         = 6.2137119223733e-4;

/**
 * @brief  Returns the radian measurement of a degree value
 *
 * @param  degrees the degree measurement
 *
 * @return the radian equivalent of the degrees
 */
double Cartesian::radiansFromDeg(double degrees)
{
  return degrees / 180 * PI;
}

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
double Cartesian::haversine(double startLat,
                            double endLat,
                            double distLat,
                            double distLng,
                            char   unit = 'm')
{
  const double a = std::pow(std::sin(distLat / 2), 2) +
                   std::cos(startLat) * std::cos(endLat) *
                       std::pow(std::sin(distLng / 2), 2);
  const double c = 2 * std::atan2(sqrt(a), std::sqrt(1 - a));
  const double d = c * EARTH_RADIUS_METERS;

  return d * (unit == 'm' ? METER_TO_KM : METER_TO_MI);
}
