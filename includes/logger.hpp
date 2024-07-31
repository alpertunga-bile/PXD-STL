#pragma once

#include "checks.hpp"

namespace pxd {
class Logger {
  Logger() = default;
  inline ~Logger() noexcept;

public:
  Logger(const Logger &other) = delete;
  auto operator=(const Logger &other) -> Logger & = delete;
  Logger(Logger &&other) = delete;
  auto operator=(Logger &&other) -> Logger & = delete;

  void log_info(const char *msg, const char *filename, int line,
                const char *function_name) noexcept;
  void log_warning(const char *msg, const char *filename, int line,
                   const char *function_name) noexcept;
  void log_error(const char *msg, const char *filename, int line,
                 const char *function_name) noexcept;

  static auto get_instance() noexcept -> Logger *;

private:
  void log(const char *log_level, const char *msg, const char *filename,
           int line, const char *func_name);

private:
  static Logger *instance;
};
} // namespace pxd

#if defined(_DEBUG) || defined(PXD_LOG_FILE_ONLY)
#define PXD_LOG_INFO(msg)                                                      \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_info(msg, __FILE__, __LINE__, PXD_CHECKS_FUNCTION_NAME);       \
  }
#define PXD_LOG_WARNING(msg)                                                   \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_warning(msg, __FILE__, __LINE__, PXD_CHECKS_FUNCTION_NAME);    \
  }
#define PXD_LOG_ERROR(msg)                                                     \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_error(msg, __FILE__, __LINE__, PXD_CHECKS_FUNCTION_NAME);      \
  }
#else
#define PXD_LOG_INFO(msg)
#define PXD_LOG_WARNING(msg)
#define PXD_LOG_ERROR(msg)
#endif