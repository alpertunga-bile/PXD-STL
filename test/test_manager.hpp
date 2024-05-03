#pragma once

#include "core.h"
#include "i_test.hpp"
#include "os.h"

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
      fmt::print(
          "---------------------------------------------------------------\n");
      fmt::print("Test Name : {}\n", test_name.c_str());

      for (auto &[test_case_name, test_result] : test) {
        fmt::print("  {:30s} -> {:6s}\n", test_case_name.c_str(),
                   test_result == true ? "Passed" : "Failed");

        check_test_counts(test_result);
      }
    }

    fmt::print(
        "---------------------------------------------------------------\n");

    float passed_ratio = static_cast<float>(passed_tests) /
                         static_cast<float>(total_tests) * 100.0f;
    fmt::print(
        " Passed Tests : {} | Total Tests : {} | Passed Ratio : {:.2f}\n",
        passed_tests, total_tests, passed_ratio);

    fmt::print(
        "---------------------------------------------------------------\n");
  }

  void save_results(const char *filename = "test_results.txt") noexcept {
    auto test_file = fmt::output_file(filename);
    reset_test_counts();

    for (auto &[test_name, test] : tests) {
      test_file.print(
          "-----------------------------------------------------------"
          "----\n");

      test_file.print("Test Name : {}\n", test_name);

      for (auto &[test_case_name, test_result] : test) {
        test_file.print("   {:30s} -> {:6s}\n", test_case_name,
                        (test_result == true ? "Passed" : "Failed"));

        check_test_counts(test_result);
      }
    }

    test_file.print(
        "-----------------------------------------------------------"
        "----\n");

    float passed_ratio = static_cast<float>(passed_tests) /
                         static_cast<float>(total_tests) * 100.f;

    test_file.print(
        " Passed Tests : {} | Total Tests : {} | Passed Ratio : {:3.2f}\n",
        passed_tests, total_tests, passed_ratio);

    test_file.print(
        "-----------------------------------------------------------"
        "----\n");

    test_file.close();
  }

private:
  std::unordered_map<std::string, std::unordered_map<std::string, bool>> tests;
  int passed_tests = 0;
  int total_tests = 0;
};
} // namespace pxd