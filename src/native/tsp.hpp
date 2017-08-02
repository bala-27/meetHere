#include <algorithm>
#include <cmath>
#include <limits>
#include <node.h>
#include <vector>

namespace TSP {

extern double cost(double[][2], unsigned int &, double &, double &);
extern short branchBound(std::vector<std::vector<double>> &, unsigned int &,
                         unsigned int &, std::vector<bool> &);

} // namespace TSP
