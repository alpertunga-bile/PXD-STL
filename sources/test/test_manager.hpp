#pragma once

#include "i_test.hpp"
#include <cstdio>

namespace pxd {
class TestManager
{
public:
  void add_test(std::string&& name, ITest& test)
  {
    test.start_test();
    tests[name] = test.test_results;
  }

  void print_results()
  {
    for (auto& [test_name, test] : tests) {
      for (auto& [test_case_name, test_result] : test) {
        printf(
          "---------------------------------------------------------------\n");
        printf("Test Name : %s\n", test_name.c_str());

        std::string result_string = test_result == true ? "Passed" : "Failed";
        printf("%30s -> %6s\n", test_case_name.c_str(), result_string.c_str());
      }
    }

    printf("---------------------------------------------------------------\n");
  }

private:
  std::map<std::string, std::map<std::string, bool>> tests;
};
}