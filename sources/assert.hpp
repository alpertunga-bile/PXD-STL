#pragma once

#ifdef assert
#undef assert
#endif

#include <cstdio>
#include <cstdlib>

#ifndef __FUNCTION_NAME__
#ifdef _WIN32
#define __FUNCTION_NAME__ __FUNCTION__
#else
#define __FUNCTION_NAME__ __func__
#endif
#endif

#define assert(exp) assert_func(exp, __FILE__, __LINE__, __FUNCTION_NAME__)

inline void assert_func(bool expression, const char *filename, int line,
                        const char *function_name) {
  if (expression) {
    return;
  }

  printf("Assertion is FAILED at:\nFilename: %s\nLine : %d\nFunction: [%s]\n",
         filename, line, function_name);
  exit(EXIT_FAILURE);
}