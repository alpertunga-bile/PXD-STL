#include "logger.hpp"

#include "core.h"
#include <filesystem>

namespace pxd {
Logger::Logger() { errno_t error = fopen_s(&log_file, log_filename, "w"); }

inline Logger::~Logger() noexcept {
  if (log_file != nullptr) {
    fclose(log_file);
  }

  delete instance;
}

void Logger::log_info(const char *msg, const char *filename, int line,
                      const char *function_name) noexcept {
  log("INFO", msg, filename, line, function_name);
}

void Logger::log_warning(const char *msg, const char *filename, int line,
                         const char *function_name) noexcept {
  log("WARNING", msg, filename, line, function_name);
}

void Logger::log_error(const char *msg, const char *filename, int line,
                       const char *function_name) noexcept {
  log("ERROR", msg, filename, line, function_name);
}

Logger *Logger::instance = nullptr;

Logger *Logger::get_instance() noexcept {
  if (instance == nullptr) {
    instance = new Logger();
  }

  return instance;
}

constexpr const char *format_str =
    "[{:8s}] /_\\ {:50s} /_\\ {:20s} /_\\ {:5d} /_\\ {}\n";

void Logger::log(const char *log_level, const char *msg, const char *filename,
                 int line, const char *func_name) {
  auto base_filename = std::filesystem::path(filename).filename().string();

  if (!just_log_file) {
    fmt::print(format_str, log_level, msg, base_filename, line, func_name);
  }
  fmt::print(log_file, format_str, log_level, msg, base_filename, line,
             func_name);
}

} // namespace pxd