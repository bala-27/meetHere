#include "util.h"
#include <limits>

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
