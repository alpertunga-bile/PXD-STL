#pragma once

#include "checks.hpp"
#include <fstream>

namespace pxd {
class Logger {
public:
  Logger();
  Logger(const Logger &other) = delete;
  Logger &operator=(const Logger &other) = delete;
  Logger(Logger &&other) = delete;
  Logger &operator=(Logger &&other) = delete;
  inline ~Logger() noexcept;

  void log_info(const char *msg, const char *filename, int line,
                const char *function_name) noexcept;
  void log_warning(const char *msg, const char *filename, int line,
                   const char *function_name) noexcept;
  void log_error(const char *msg, const char *filename, int line,
                 const char *function_name) noexcept;

  static Logger *get_instance() noexcept;

private:
  std::ofstream log_file;
  static Logger *instance;
};
} // namespace pxd

#if defined(_DEBUG)
#define LOG_INFO(msg)                                                          \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_info(msg, __FILE__, __LINE__, __FUNCTION_NAME__);              \
  }
#define LOG_WARNING(msg)                                                       \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_warning(msg, __FILE__, __LINE__, __FUNCTION_NAME__);           \
  }
#define LOG_ERROR(msg)                                                         \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_error(msg, __FILE__, __LINE__, __FUNCTION_NAME__);             \
  }

#else
#define LOG_INFO(msg)
#define LOG_WARNING(msg)
#define LOG_ERROR(msg)
#endif