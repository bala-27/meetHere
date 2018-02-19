#ifndef UTIL_H
#define UTIL_H

#include <cmath>

namespace Util
{
typedef double *  DoubleArr;
typedef double ** DoubleArr2D;

/**
 * @brief  Checks if an array contains a certain value
 *
 * @tparam T   type of value to check for
 * @param  arr array to check
 * @param  len length of array to check
 * @param  val value to check for
 *
 * @return whether the value exists
 */
template <class T>
bool arr_contains(const T * arr, size_t len, T val);
}  // namespace Util

#include "util.cpp"

#endif
