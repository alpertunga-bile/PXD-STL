#pragma once

#include "checks.hpp"
#include <filesystem>

namespace pxd {
class Logger {
public:
  constexpr Logger();
  Logger(const Logger &other) = delete;
  Logger &operator=(const Logger &other) = delete;
  Logger(Logger &&other) = delete;
  Logger &operator=(Logger &&other) = delete;
  inline ~Logger() noexcept;

  constexpr void log_info(const char *msg, const char *time,
                          const char *filename, int line,
                          const char *function_name) noexcept;
  constexpr void log_warning(const char *msg, const char *time,
                             const char *filename, int line,
                             const char *function_name) noexcept;
  constexpr void log_error(const char *msg, const char *time,
                           const char *filename, int line,
                           const char *function_name) noexcept;

  static Logger *get_instance() noexcept;

private:
  void write_to_log_file(const char *log_string) noexcept;

private:
  std::ofstream log_file;
  static Logger *instance;
};

Logger *logger = Logger::get_instance();
} // namespace pxd

#if defined(_DEBUG)
#define LOG_INFO(msg)                                                          \
  logger->log_info(msg, __TIME__, __FILE__, __LINE__, __FUNCTION_NAME__)
#define LOG_WARNING(msg)                                                       \
  logger->log_warning(msg, __TIME__, __FILE__, __LINE__, __FUNCTION_NAME__)
#define LOG_ERROR(msg)                                                         \
  logger->log_error(msg, __TIME__, __FILE__, __LINE__, __FUNCTION_NAME__)
#else
#define LOG_INFO(msg)
#define LOG_WARNING(msg)
#define LOG_ERROR(msg)
#endif