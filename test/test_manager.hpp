#pragma once

#include "i_test.hpp"
#include <cstdio>
#include <fstream>
#include <iomanip>

namespace pxd {
class TestManager {
public:
  void add_test(std::string &&name, ITest &test) {
    test.start_test();
    tests[name] = test.test_results;
  }

  void print_results() {
    for (auto &[test_name, test] : tests) {
      printf(
          "---------------------------------------------------------------\n");
      printf("Test Name : %s\n", test_name.c_str());

      for (auto &[test_case_name, test_result] : test) {
        printf("  %-30s -> %-6s\n", test_case_name.c_str(),
               test_result == true ? "Passed" : "Failed");
      }
    }

    printf("---------------------------------------------------------------\n");
  }

  void save_results(const char *filename = "test_results.txt") {
    std::ofstream test_file(filename, std::ios::out);

    for (auto &[test_name, test] : tests) {
      test_file << "-----------------------------------------------------------"
                   "----\n";

      test_file << "Test Name : " << test_name << "\n";

      for (auto &[test_case_name, test_result] : test) {
        test_file << "   " << std::left << std::setw(30) << test_case_name
                  << " -> " << std::left << std::setw(6)
                  << (test_result == true ? "Passed" : "Failed") << "\n";
      }
    }

    test_file << "-----------------------------------------------------------"
                 "----\n";

    test_file.close();
  }

private:
  std::unordered_map<std::string, std::unordered_map<std::string, bool>> tests;
};
} // namespace pxd