#include "logger.hpp"

#include "core.h"
#include <filesystem>

namespace pxd {
Logger::Logger() {}

inline Logger::~Logger() noexcept
{
  log_file.close();

  delete instance;
  instance = nullptr;
}

void
Logger::log_info(const char* msg,
                 const char* filename,
                 int line,
                 const char* function_name) noexcept
{
  log("INFO", msg, filename, line, function_name);
}

void
Logger::log_warning(const char* msg,
                    const char* filename,
                    int line,
                    const char* function_name) noexcept
{
  log("WARNING", msg, filename, line, function_name);
}

void
Logger::log_error(const char* msg,
                  const char* filename,
                  int line,
                  const char* function_name) noexcept
{
  log("ERROR", msg, filename, line, function_name);
}

Logger* Logger::instance = nullptr;

Logger*
Logger::get_instance() noexcept
{
  if (instance == nullptr) {
    instance = new Logger();
  }

  return instance;
}

constexpr const char* format_str =
  "[{:8s}] /_\\ {:50s} /_\\ {:20s} /_\\ {:5d} /_\\ {}\n";

void
Logger::log(const char* log_level,
            const char* msg,
            const char* filename,
            int line,
            const char* func_name)
{
  auto base_filename = std::filesystem::path(filename).filename().string();

#ifndef PXD_LOG_FILE_ONLY
  fmt::print(format_str, log_level, msg, base_filename, line, func_name);
#endif

  log_file.print(format_str, log_level, msg, base_filename, line, func_name);
}

} // namespace pxd