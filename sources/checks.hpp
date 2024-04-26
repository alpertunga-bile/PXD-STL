#pragma once

#include <cstdlib>
#include <iostream>

#ifndef __FUNCTION_NAME__
#ifdef _WIN32
#define __FUNCTION_NAME__ __FUNCTION__
#else
#define __FUNCTION_NAME__ __func__
#endif
#endif

#define IS_VALID(obj) (obj == nullptr ? false : true)

#ifdef ENABLE_ASSERTS
#define PXD_ASSERT(exp) assert_func(exp, __FILE__, __LINE__, __FUNCTION_NAME__)
#define PXD_ASSERT_MSG(exp, msg)                                               \
  assert_msg_func(msg, exp, __FILE__, __LINE__, __FUNCTION_NAME__)
#define PXD_TODO(msg) todo_func(msg, __FILE__, __LINE__, __FUNCTION_NAME__)
#else
#define PXD_ASSERT(exp)
#define PXD_ASSERT_MSG(exp, msg)
#define PXD_TODO(msg)
#endif

inline void assert_func(bool expression, const char *filename, int line,
                        const char *function_name) {
  if (expression) {
    return;
  }

  printf("Assertion is FAILED at:\n    Filename : %s\n    Line     : %d\n    "
         "Function : "
         "%s\n",
         filename, line, function_name);
  exit(EXIT_FAILURE);
}

inline void todo_func(const char *message, const char *filename, int line,
                      const char *function_name) {
  printf("TODO message:\n    Message : %s\n    Filename : %s\n    Line     : "
         "%d\n    "
         "Function : "
         "%s\n",
         message, filename, line, function_name);
  exit(EXIT_FAILURE);
}

inline void assert_msg_func(const char *message, bool expression,
                            const char *filename, int line,
                            const char *function_name) {
  if (expression) {
    return;
  }

  printf("Assert is FAILED at: message:\n    Message : %s\n    Filename : %s\n "
         "   Line     : "
         "%d\n    "
         "Function : "
         "%s\n",
         message, filename, line, function_name);
  exit(EXIT_FAILURE);
}