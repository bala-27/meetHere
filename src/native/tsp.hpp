#include <cmath>
#include <limits>
#include <node.h>

namespace TSP {

extern short branchBound(double **, unsigned int &, unsigned int &, bool *);
extern double cost(double **, unsigned int &, double &, double &);
extern double manhattanCost(double **, unsigned int &, double &, double &);

} // namespace TSP
