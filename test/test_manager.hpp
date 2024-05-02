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

  constexpr void reset_test_counts() noexcept {
    total_tests = 0;
    passed_tests = 0;
  }

  void check_test_counts(bool result) noexcept {
    total_tests++;
    passed_tests = result ? passed_tests + 1 : 0;
  }

  void print_results() noexcept {
    reset_test_counts();

    for (auto &[test_name, test] : tests) {
      printf(
          "---------------------------------------------------------------\n");
      printf("Test Name : %s\n", test_name.c_str());

      for (auto &[test_case_name, test_result] : test) {
        printf("  %-30s -> %-6s\n", test_case_name.c_str(),
               test_result == true ? "Passed" : "Failed");

        check_test_counts(test_result);
      }
    }

    printf("---------------------------------------------------------------\n");

    float passed_ratio = static_cast<float>(passed_tests) /
                         static_cast<float>(total_tests) * 100.0f;
    printf(" Passed Tests : %d | Total Tests : %d | Passed Ratio : %.2f\n",
           passed_tests, total_tests, passed_ratio);

    printf("---------------------------------------------------------------\n");
  }

  void save_results(const char *filename = "test_results.txt") noexcept {
    std::ofstream test_file(filename, std::ios::out);
    reset_test_counts();

    for (auto &[test_name, test] : tests) {
      test_file << "-----------------------------------------------------------"
                   "----\n";

      test_file << "Test Name : " << test_name << "\n";

      for (auto &[test_case_name, test_result] : test) {
        test_file << "   " << std::left << std::setw(30) << test_case_name
                  << " -> " << std::left << std::setw(6)
                  << (test_result == true ? "Passed" : "Failed") << "\n";

        check_test_counts(test_result);
      }
    }

    test_file << "-----------------------------------------------------------"
                 "----\n";

    float passed_ratio = static_cast<float>(passed_tests) /
                         static_cast<float>(total_tests) * 100.f;

    test_file << " Passed Tests : " << passed_tests
              << " | Total Tests : " << total_tests
              << " | Passed Ratio : " << passed_ratio << "\n";

    test_file << "-----------------------------------------------------------"
                 "----\n";

    test_file.close();
  }

private:
  std::unordered_map<std::string, std::unordered_map<std::string, bool>> tests;
  int passed_tests = 0;
  int total_tests = 0;
};
} // namespace pxd