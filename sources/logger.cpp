#include "logger.hpp"

#include "string.hpp"

#include <format>

namespace pxd {
String get_output_string(const char *filename, int line,
                         const char *function_name) {
  auto base_filename = std::filesystem::path(filename).filename().string();

  auto formattedString =
      std::format("{} /_\\ {} /_\\ {}", base_filename, line, function_name);

  return String(formattedString);
}

constexpr Logger::Logger() {
  log_file.open("app.log");
  log_file.close();

  log_file.open("app.log", std::ios_base::app);
}

inline Logger::~Logger() noexcept {
  if (log_file.is_open()) {
    log_file.close();
  }

  delete instance;
}

constexpr void Logger::log_info(const char *msg, const char *time,
                                const char *filename, int line,
                                const char *function_name) noexcept {
  std::string formatStr =
      std::format("[{}] /_\\ [    INFO] /_\\ {} /_\\ {}\n", time, msg,
                  get_output_string(filename, line, function_name).c_str());

  printf("%s", formatStr.c_str());
  write_to_log_file(formatStr.c_str());
}

constexpr void Logger::log_warning(const char *msg, const char *time,
                                   const char *filename, int line,
                                   const char *function_name) noexcept {
  std::string formatStr =
      std::format("[{}] /_\\ [ WARNING] /_\\ {} /_\\ {}\n", time, msg,
                  get_output_string(filename, line, function_name).c_str());

  printf("%s", formatStr.c_str());
  write_to_log_file(formatStr.c_str());
}

constexpr void Logger::log_error(const char *msg, const char *time,
                                 const char *filename, int line,
                                 const char *function_name) noexcept {
  std::string formatStr =
      std::format("[{}] /_\\ [  FAILED] /_\\ {} /_\\ {}\n", time, msg,
                  get_output_string(filename, line, function_name).c_str());

  printf("%s", formatStr.c_str());
  write_to_log_file(formatStr.c_str());
}

Logger *Logger::get_instance() {
  if (instance == nullptr) {
    instance = new Logger();
  }

  return instance;
}

void Logger::write_to_log_file(const char *log_string) noexcept {
  log_file << log_string;
}

} // namespace pxd