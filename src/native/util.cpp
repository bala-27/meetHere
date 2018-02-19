#include "util.h"
#include <limits>

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
bool Util::arr_contains(const T * arr, size_t len, T val)
{
  for (size_t i = 0; i < len; ++i) {
    if (arr[i] == val) {
      return true;
    }
  }
  return false;
}
