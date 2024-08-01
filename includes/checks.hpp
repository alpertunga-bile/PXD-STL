#pragma once

#ifndef PXD_CHECKS_FUNCTION_NAME
#ifdef _WIN32
#define PXD_CHECKS_FUNCTION_NAME __FUNCTION__
#else
#define PXD_CHECKS_FUNCTION_NAME __func__
#endif
#endif

constexpr auto IS_VALID_PTR(void *obj) -> bool { return obj != nullptr; }

#if defined(PXD_ENABLE_ASSERTS) || defined(_DEBUG)
#define PXD_ASSERT(exp)                                                        \
  assert_func(exp, __FILE__, __LINE__, PXD_CHECKS_FUNCTION_NAME)
#define PXD_ASSERT_MSG(exp, msg)                                               \
  assert_msg_func(msg, exp, __FILE__, __LINE__, PXD_CHECKS_FUNCTION_NAME)
#define PXD_TODO(msg)                                                          \
  todo_func(msg, __FILE__, __LINE__, PXD_CHECKS_FUNCTION_NAME)
#else
#define PXD_ASSERT(exp)
#define PXD_ASSERT_MSG(exp, msg)
#define PXD_TODO(msg)
#endif

void assert_func(bool expression, const char *filename, int line,
                 const char *function_name);

void todo_func(const char *message, const char *filename, int line,
               const char *function_name);

void assert_msg_func(const char *message, bool expression, const char *filename,
                     int line, const char *function_name);