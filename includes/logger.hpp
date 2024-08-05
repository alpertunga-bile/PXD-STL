#pragma once

#include "checks.hpp"

#include "../third-party/fmt/include/fmt/format.h"

namespace pxd {
class Logger {
  Logger() = default;
  inline ~Logger() noexcept;

public:
  Logger(const Logger &other) = delete;
  auto operator=(const Logger &other) -> Logger & = delete;
  Logger(Logger &&other) = delete;
  auto operator=(Logger &&other) -> Logger & = delete;

  template <typename... T>
  void log_info(fmt::format_string<T...> msg, const char *filename, int line,
                const char *function_name, T &&...args) noexcept {
    log("INFO", msg, filename, line, function_name,
        fmt::make_format_args(args...));
  }

  template <typename... T>
  void log_warning(fmt::format_string<T...> msg, const char *filename, int line,
                   const char *function_name, T &&...args) noexcept {
    log("WARNING", msg, filename, line, function_name,
        fmt::make_format_args(args...));
  }

  template <typename... T>
  void log_error(fmt::format_string<T...> msg, const char *filename, int line,
                 const char *function_name, T &&...args) noexcept {
    log("ERROR", msg, filename, line, function_name,
        fmt::make_format_args(args...));
  }

  static auto get_instance() noexcept -> Logger *;

private:
  void log(const char *log_level, fmt::string_view msg, const char *filename,
           int line, const char *func_name, fmt::format_args args);

private:
  static Logger *instance;
};
} // namespace pxd

#if defined(_DEBUG) || defined(PXD_LOG_FILE_ONLY)
#define PXD_LOG_INFO(msg, ...)                                                 \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_info(msg, __FILE__, __LINE__, PXD_CHECKS_FUNCTION_NAME,        \
                     __VA_ARGS__);                                             \
  }
#define PXD_LOG_WARNING(msg, ...)                                              \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_warning(msg, __FILE__, __LINE__, PXD_CHECKS_FUNCTION_NAME,     \
                        __VA_ARGS__);                                          \
  }
#define PXD_LOG_ERROR(msg, ...)                                                \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_error(msg, __FILE__, __LINE__, PXD_CHECKS_FUNCTION_NAME,       \
                      __VA_ARGS__);                                            \
  }
#else
#define PXD_LOG_INFO(msg)
#define PXD_LOG_WARNING(msg)
#define PXD_LOG_ERROR(msg)
#endif