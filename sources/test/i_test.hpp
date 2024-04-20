#pragma once

#include "../checks.hpp"
#include <map>
#include <string>

namespace pxd {
class ITest
{
public:
  virtual void start_test() { PXD_ASSERT(false); };

  std::map<std::string, bool> test_results;
};
}