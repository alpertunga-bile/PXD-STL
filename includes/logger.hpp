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

  constexpr inline void set_just_log_file(bool log_bool) noexcept {
    just_log_file = log_bool;
  }

private:
  void log(const char *log_level, const char *msg, const char *filename,
           int line, const char *func_name);

private:
  FILE *log_file = nullptr;
  const char *log_filename = "app.log";
  static Logger *instance;

#ifdef PXD_LOG_FILE_ONLY
  bool just_log_file = true;
#else
  bool just_log_file = false;
#endif
};
} // namespace pxd

#if defined(_DEBUG)
#define PXD_LOG_INFO(msg)                                                      \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_info(msg, __FILE__, __LINE__, __FUNCTION_NAME__);              \
  }
#define PXD_LOG_WARNING(msg)                                                   \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_warning(msg, __FILE__, __LINE__, __FUNCTION_NAME__);           \
  }
#define PXD_LOG_ERROR(msg)                                                     \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_error(msg, __FILE__, __LINE__, __FUNCTION_NAME__);             \
  }
#else
#ifdef PXD_LOG_FILE_ONLY
#define PXD_LOG_INFO(msg)                                                      \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_info(msg, __FILE__, __LINE__, __FUNCTION_NAME__);              \
  }
#define PXD_LOG_WARNING(msg)                                                   \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_warning(msg, __FILE__, __LINE__, __FUNCTION_NAME__);           \
  }
#define PXD_LOG_ERROR(msg)                                                     \
  {                                                                            \
    pxd::Logger *logger = pxd::Logger::get_instance();                         \
    logger->log_error(msg, __FILE__, __LINE__, __FUNCTION_NAME__);             \
  }
#else
#define PXD_LOG_INFO(msg)
#define PXD_LOG_WARNING(msg)
#define PXD_LOG_ERROR(msg)
#endif
#endif