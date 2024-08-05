#include "filesystem.hpp"

#include <chrono>
#include <filesystem>

#include "string.hpp"

#include "logger.hpp"

#include "core.h"

namespace pxd::fs {
auto exists(const char *path) -> bool { return std::filesystem::exists(path); }

auto is_dir(const char *path) -> bool {
  return std::filesystem::is_directory(path);
}

auto is_file(const char *path) -> bool {
  return std::filesystem::is_regular_file(path);
}

void mkdir(const char *path) {
  if (!exists(path)) {
    return;
  }

  std::filesystem::create_directory(path);
}

auto get_file_hash(const char *path) -> size_t {
  if (!exists(path)) {
    return 0;
  }

  return std::filesystem::hash_value(path);
}

auto get_relative_path(const char *path) -> String {
  return String(std::filesystem::relative(path).string());
}

auto get_absolute_path(const char *path) -> String {
  return String(std::filesystem::absolute(path).string());
}

auto create_file_symlink(const char *filepath, const char *symlink) -> bool {
  if (!exists(filepath)) {
    return false;
  }

  try {
    std::filesystem::create_symlink(filepath, symlink);
  } catch (const std::exception &e) {
    PXD_LOG_ERROR("Creating {} symlink for {} file is failed", symlink,
                  filepath);
    PXD_LOG_ERROR("{}", e.what());
    return false;
  }

  return true;
}

auto create_directory_symlink(const char *dirpath,
                              const char *symlink) -> bool {
  if (!exists(dirpath)) {
    return false;
  }

  try {
    std::filesystem::create_directory_symlink(dirpath, symlink);
  } catch (const std::exception &e) {
    PXD_LOG_ERROR("Creating {} symlink for {} directory is failed", symlink,
                  dirpath);
    PXD_LOG_ERROR("{}", e.what());
    return false;
  }

  return true;
}

auto getcwd() -> String {
  return String(std::filesystem::current_path().string());
}

auto get_last_modified_time(const char *path) -> String {
  if (!exists(path)) {
    return {};
  }

  auto modified_time = std::filesystem::last_write_time(path);

  try {
    auto system_time =
        std::chrono::clock_cast<std::chrono::system_clock>(modified_time);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return {};
}

auto remove_file(const char *path) -> bool {
  if (!exists(path)) {
    return false;
  }

  if (!is_file(path)) {
    return false;
  }

  return std::filesystem::remove(path);
}

auto remove_folder(const char *path) -> bool {
  if (!exists(path)) {
    return false;
  }

  if (!is_dir(path)) {
    return false;
  }

  return std::filesystem::remove_all(path) > 0 ? true : false;
}

auto get_temp_dir_path() -> String {
  return String(std::filesystem::temp_directory_path().string());
}

void copy_dir(const char *from_dir, const char *to_dir, bool is_recursive,
              bool update_existed) {
  if (!exists(from_dir)) {
    return;
  }

  std::filesystem::copy_options copy_ops;

  if (is_recursive) {
    copy_ops |= std::filesystem::copy_options::recursive;
  }

  if (update_existed) {
    copy_ops |= std::filesystem::copy_options::update_existing;
  }

  std::filesystem::copy(from_dir, to_dir, copy_ops);
}

void copy_file(const char *from_file, const char *to_file,
               bool update_existed) {
  if (!exists(from_file)) {
    return;
  }

  std::filesystem::copy_options copy_ops;

  if (update_existed) {
    copy_ops |= std::filesystem::copy_options::update_existing;
  }

  std::filesystem::copy_file(from_file, to_file, copy_ops);
}

void rename(const char *_old, const char *_new) {
  if (!exists(_old)) {
    return;
  }

  if (exists(_new)) {
    return;
  }

  std::filesystem::rename(_old, _new);
}

} // namespace pxd::fs

namespace pxd::fs::path {
auto remove_filename(const char *path) -> String {
  return String(std::filesystem::path(path).remove_filename().string());
}

auto replace_filename(const char *path, const char *new_filename) -> String {
  return String(
      std::filesystem::path(path).replace_filename(new_filename).string());
}

auto replace_extension(const char *path, const char *new_ext) -> String {
  return String(
      std::filesystem::path(path).replace_extension(new_ext).string());
}

auto get_root_name(const char *path) -> String {
  return String(std::filesystem::path(path).root_name().string());
}

auto get_root_directory(const char *path) -> String {
  return String(std::filesystem::path(path).root_directory().string());
}

auto get_root_path(const char *path) -> String {
  return String(std::filesystem::path(path).root_path().string());
}

auto get_relative_path(const char *path) -> String {
  return String(std::filesystem::path(path).relative_path().string());
}

auto get_parent_path(const char *path) -> String {
  return String(std::filesystem::path(path).parent_path().string());
}

auto get_filename(const char *path) -> String {
  return String(std::filesystem::path(path).filename().string());
}

auto get_filename_wo_ext(const char *path) -> String {
  return String(std::filesystem::path(path).stem().string());
}

auto get_extension_w_dot(const char *path) -> String {
  return String(std::filesystem::path(path).stem().string());
}

} // namespace pxd::fs::path
