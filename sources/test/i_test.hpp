#pragma once

#include "../checks.hpp"
#include <string>
#include <unordered_map>

namespace pxd {
class ITest
{
public:
  virtual void start_test() { PXD_ASSERT(false); };

  std::unordered_map<std::string, bool> test_results;
};
}