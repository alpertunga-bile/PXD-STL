#include "checks.hpp"

#include "core.h" // fmt/core.h
#include <cassert>
#include <stdlib.h> // don't need the math.h lib so not using cstdlib

void assert_func(bool expression, const char *filename, int line,
                 const char *function_name) {
  if (expression) {
    return;
  }

  fmt::print(
      "Assertion is FAILED at:\n    Filename : {}\n    Line     : {}\n    "
      "Function : "
      "{}\n",
      filename, line, function_name);

  assert(false);
}

void todo_func(const char *message, const char *filename, int line,
               const char *function_name) {
  fmt::print(
      "TODO message:\n    Message : {}\n    Filename : {}\n    Line     : "
      "{}\n    "
      "Function : "
      "{}\n",
      message, filename, line, function_name);

  assert(false);
}

void assert_msg_func(const char *message, bool expression, const char *filename,
                     int line, const char *function_name) {
  if (expression) {
    return;
  }

  fmt::print(
      "Assert is FAILED at: message:\n    Message : {}\n    Filename : {}\n "
      "   Line     : "
      "{}\n    "
      "Function : "
      "{}\n",
      message, filename, line, function_name);

  assert(false);
}