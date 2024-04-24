#include "logger.hpp"

#include "string.hpp"

#include <filesystem>
#include <format>
#include <fstream>

void create_log_file() {
  if (std::filesystem::exists(std::filesystem::path("app.log"))) {
    std::filesystem::remove(std::filesystem::path("app.log"));
  } else {
    std::ofstream log_file("app.log");
    log_file.close();
  }
}

void write_to_log_file(const char *log_string) {
  std::ofstream log_file("app.log", std::ios_base::app);
  log_file << log_string;
  log_file.close();
}

String get_output_string(const char *filename, int line,
                         const char *function_name) {
  auto base_filename = std::filesystem::path(filename).filename().string();

  auto formattedString =
      std::format("{} /_\\ {} /_\\ {}", base_filename, line, function_name);

  return String(formattedString);
}

void log_info(const char *msg, const char *time, const char *filename, int line,
              const char *function_name) {
  std::string formatStr =
      std::format("[{}] /_\\ [    INFO] /_\\ {} /_\\ {}\n", time, msg,
                  get_output_string(filename, line, function_name).c_str());

  printf("%s", formatStr.c_str());
  write_to_log_file(formatStr.c_str());
}

void log_warning(const char *msg, const char *time, const char *filename,
                 int line, const char *function_name) {
  std::string formatStr =
      std::format("[{}] /_\\ [ WARNING] /_\\ {} /_\\ {}\n", time, msg,
                  get_output_string(filename, line, function_name).c_str());

  printf("%s", formatStr.c_str());
  write_to_log_file(formatStr.c_str());
}

void log_error(const char *msg, const char *time, const char *filename,
               int line, const char *function_name) {
  std::string formatStr =
      std::format("[{}] /_\\ [  FAILED] /_\\ {} /_\\ {}\n", time, msg,
                  get_output_string(filename, line, function_name).c_str());

  printf("%s", formatStr.c_str());
  write_to_log_file(formatStr.c_str());
}