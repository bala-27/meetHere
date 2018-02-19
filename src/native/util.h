#ifndef UTIL_H
#define UTIL_H

#include <cmath>

namespace Util
{
typedef double *  DoubleArr;
typedef double ** DoubleArr2D;

template <class T>
bool arr_contains(const T * arr, size_t len, T val);
}  // namespace Util

#include "util.cpp"

#endif
