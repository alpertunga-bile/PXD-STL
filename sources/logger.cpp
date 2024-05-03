#include "logger.hpp"

#include "string.hpp"

#include <filesystem>
#include <format>

const String log_filename = "app.log";

namespace pxd {
String get_output_string(const char *filename, int line,
                         const char *function_name) {
  auto base_filename = std::filesystem::path(filename).filename().string();

  auto formattedString =
      std::format("{} /_\\ {} /_\\ {}", base_filename, line, function_name);

  return String(formattedString);
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
  auto formatStr =
      std::format("[    INFO] /_\\ {} /_\\ {}\n", msg,
                  get_output_string(filename, line, function_name).c_str());

  printf("%s", formatStr.c_str());
  log_file << formatStr;
}

void Logger::log_warning(const char *msg, const char *filename, int line,
                         const char *function_name) noexcept {
  auto formatStr =
      std::format("[ WARNING] /_\\ {} /_\\ {}\n", msg,
                  get_output_string(filename, line, function_name).c_str());

  printf("%s", formatStr.c_str());
  log_file << formatStr;
}

void Logger::log_error(const char *msg, const char *filename, int line,
                       const char *function_name) noexcept {
  auto formatStr =
      std::format("[  FAILED] /_\\ {} /_\\ {}\n", msg,
                  get_output_string(filename, line, function_name).c_str());

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