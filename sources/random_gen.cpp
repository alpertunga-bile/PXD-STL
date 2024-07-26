#include "random_gen.hpp"

#include <random>

std::random_device rd;
std::mt19937 generator(rd());

namespace pxd {
double get_random_value(double min, double max) {
  std::uniform_real_distribution<> dist(min, max);

  return dist(generator);
}
} // namespace pxd