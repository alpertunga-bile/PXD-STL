#include "logger.hpp"

#include "string.hpp"

#include <filesystem>
#include <format>

const String log_filename = "app.log";

namespace pxd {
String get_log_string(const char *log_level, const char *msg,
                      const char *filename, int line,
                      const char *func_name) noexcept {
  auto base_filename = std::filesystem::path(filename).filename().string();

  return String(
      std::format("[{:8s}] /_\\ {:50s} /_\\ {:20s} /_\\ {:5d} /_\\ {}\n",
                  log_level, msg, base_filename, line, func_name));
}

Logger::Logger() { log_file.open(log_filename.c_str()); }

inline Logger::~Logger() noexcept {
  if (log_file.is_open()) {
    log_file.close();
  }

  delete instance;
}

void Logger::log_info(const char *msg, const char *filename, int line,
                      const char *function_name) noexcept {
  auto formatStr = get_log_string("INFO", msg, filename, line, function_name);

  printf("%s", formatStr.c_str());
  log_file << formatStr;
}

void Logger::log_warning(const char *msg, const char *filename, int line,
                         const char *function_name) noexcept {
  auto formatStr =
      get_log_string("WARNING", msg, filename, line, function_name);

  printf("%s", formatStr.c_str());
  log_file << formatStr;
}

void Logger::log_error(const char *msg, const char *filename, int line,
                       const char *function_name) noexcept {
  auto formatStr = get_log_string("ERROR", msg, filename, line, function_name);

  printf("%s", formatStr.c_str());
  log_file << formatStr;
}

Logger *Logger::instance = nullptr;

Logger *Logger::get_instance() noexcept {
  if (instance == nullptr) {
    instance = new Logger();
  }

  return instance;
}

} // namespace pxd