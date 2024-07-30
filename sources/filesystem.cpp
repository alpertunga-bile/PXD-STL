#include "filesystem.hpp"

#include <chrono>
#include <filesystem>

#include "string.hpp"

#include "logger.hpp"

#include "core.h"

namespace pxd {
bool exists(const char *path) { return std::filesystem::exists(path); }

bool is_dir(const char *path) { return std::filesystem::is_directory(path); }

bool is_file(const char *path) {
  return std::filesystem::is_regular_file(path);
}

void mkdir(const char *path) {
  if (!exists(path)) {
    return;
  }

  std::filesystem::create_directory(path);
}

size_t get_file_hash(const char *path) {
  if (!exists(path)) {
    return 0;
  }

  return std::filesystem::hash_value(path);
}

String get_relative_path(const char *path) {
  return String(std::filesystem::relative(path).string());
}

String get_absolute_path(const char *path) {
  return String(std::filesystem::absolute(path).string());
}

bool create_file_symlink(const char *filepath, const char *symlink) {
  if (!exists(filepath)) {
    return false;
  }

  try {
    std::filesystem::create_symlink(filepath, symlink);
  } catch (const std::exception &e) {
    PXD_LOG_ERROR(fmt::format("Creating {} symlink for {} file is failed",
                              symlink, filepath)
                      .c_str());
    PXD_LOG_ERROR(e.what());
    return false;
  }

  return true;
}

bool create_directory_symlink(const char *dirpath, const char *symlink) {
  if (!exists(dirpath)) {
    return false;
  }

  try {
    std::filesystem::create_directory_symlink(dirpath, symlink);
  } catch (const std::exception &e) {
    PXD_LOG_ERROR(fmt::format("Creating {} symlink for {} directory is failed",
                              symlink, dirpath)
                      .c_str());
    PXD_LOG_ERROR(e.what());
    return false;
  }

  return true;
}

String getcwd() { return String(std::filesystem::current_path().string()); }

String get_last_modified_time(const char *path) {
  if (!exists(path)) {
    return String();
  }

  auto modified_time = std::filesystem::last_write_time(path);

  try {
    auto system_time =
        std::chrono::clock_cast<std::chrono::system_clock>(modified_time);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return String();
}

bool remove_file(const char *path) {
  if (!exists(path)) {
    return false;
  }

  if (!is_file(path)) {
    return false;
  }

  return std::filesystem::remove(path);
}

bool remove_folder(const char *path) {
  if (!exists(path)) {
    return false;
  }

  if (!is_dir(path)) {
    return false;
  }

  return std::filesystem::remove_all(path) > 0 ? true : false;
}

String get_temp_dir_path() {
  return String(std::filesystem::temp_directory_path().string());
}

void copy_dir(const char *from, const char *to, bool is_recursive,
              bool update_existed) {
  if (!exists(from)) {
    return;
  }

  std::filesystem::copy_options copy_ops;

  if (is_recursive) {
    copy_ops |= std::filesystem::copy_options::recursive;
  }

  if (update_existed) {
    copy_ops |= std::filesystem::copy_options::update_existing;
  }

  std::filesystem::copy(from, to, copy_ops);
}

void copy_file(const char *from, const char *to, bool update_existed) {
  if (!exists(from)) {
    return;
  }

  std::filesystem::copy_options copy_ops;

  if (update_existed) {
    copy_ops |= std::filesystem::copy_options::update_existing;
  }

  std::filesystem::copy_file(from, to, copy_ops);
}

} // namespace pxd