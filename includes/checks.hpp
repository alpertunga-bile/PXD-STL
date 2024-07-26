#pragma once

#ifndef __FUNCTION_NAME__
#ifdef _WIN32
#define __FUNCTION_NAME__ __FUNCTION__
#else
#define __FUNCTION_NAME__ __func__
#endif
#endif

#define IS_VALID(obj) (obj == nullptr ? false : true)

#ifdef PXD_ENABLE_ASSERTS
#define PXD_ASSERT(exp) assert_func(exp, __FILE__, __LINE__, __FUNCTION_NAME__)
#define PXD_ASSERT_MSG(exp, msg)                                               \
  assert_msg_func(msg, exp, __FILE__, __LINE__, __FUNCTION_NAME__)
#define PXD_TODO(msg) todo_func(msg, __FILE__, __LINE__, __FUNCTION_NAME__)
#else
#define PXD_ASSERT(exp)
#define PXD_ASSERT_MSG(exp, msg)
#define PXD_TODO(msg)
#endif

constexpr inline void assert_func(bool expression, const char *filename,
                                  int line, const char *function_name);

constexpr inline void todo_func(const char *message, const char *filename,
                                int line, const char *function_name);

constexpr inline void assert_msg_func(const char *message, bool expression,
                                      const char *filename, int line,
                                      const char *function_name);