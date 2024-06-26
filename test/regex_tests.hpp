#pragma once

#include "regex.hpp"
#include "test_utils.hpp"

namespace pxd {
class RegexTests : public ITest {
public:
  void start_test() override {
    start_full_match_tests();
    start_partial_match_tests();
    start_replace_tests();
    start_quote_tests();
  }

private:
  void start_full_match_tests() {
    String test_str("hello");

    RE2 true_regex("h.*o");
    RE2 false_regex("e");

    test_results["full match"] = pxd::full_match(test_str, true_regex) &&
                                 !pxd::full_match(test_str, false_regex);

    RE2 extract_regex("(\\w+):(\\d+)");

    int i;
    std::string s;
    test_str = "ruby:1234";

    pxd::full_match(test_str, extract_regex, &s, &i);

    test_results["full match extract"] = s == "ruby" && i == 1234;
  }

  void start_partial_match_tests() {
    String test_str("hello");

    RE2 true_regex("ell");

    test_results["partial match"] = pxd::partial_match(test_str, true_regex);

    test_str = "x*100 + 20";
    RE2 extract_regex("(\\d+)");

    int number;

    pxd::partial_match(test_str, extract_regex, &number);

    test_results["partial match extract"] = number == 100;
  }

  void start_replace_tests() {
    String test_str("yabba dabba doo");
    RE2 regex("b+");

    pxd::replace_first(regex, test_str, pxd::to_string("d"));

    test_results["replace first"] = test_str == "yada dabba doo";

    test_str = "yabba dabba doo";

    pxd::replace_all(regex, test_str, pxd::to_string("d"));

    test_results["replace all"] = test_str == "yada dada doo";
  }

  void start_quote_tests() {
    String test_str("1.5-2.0?");

    test_results["quote"] =
        pxd::get_escaped_string(test_str) == "1\\.5\\-2\\.0\\?";
  }
};
} // namespace pxd