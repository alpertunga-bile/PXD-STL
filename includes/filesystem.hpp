#pragma once

#include "absl/str_join.hpp"
#include <array>

namespace pxd {
class String;
} // namespace pxd

namespace pxd {

// pxd::fs
namespace fs {
auto exists(const char *path) -> bool;
auto is_dir(const char *path) -> bool;
auto is_file(const char *path) -> bool;
void mkdir(const char *path);
auto get_file_hash(const char *path) -> size_t;
auto get_relative_path(const char *path) -> String;
auto get_absolute_path(const char *path) -> String;
auto create_file_symlink(const char *filepath, const char *symlink) -> bool;
auto create_directory_symlink(const char *dirpath, const char *symlink) -> bool;
auto getcwd() -> String;
auto get_last_modified_time(const char *path) -> String;
auto remove_file(const char *path) -> bool;
auto remove_folder(const char *path) -> bool;
auto get_temp_dir_path() -> String;
void copy_dir(const char *from_dir, const char *to_dir,
              bool is_recursive = true, bool update_existed = true);
void copy_file(const char *from_file, const char *to_file,
               bool update_existed = true);
void rename(const char *_old, const char *_new);

// pxd::fs::path
namespace path {
auto remove_filename(const char *path) -> String;
auto replace_filename(const char *path, const char *new_filename) -> String;
auto replace_extension(const char *path, const char *new_ext) -> String;
auto get_root_name(const char *path) -> String;
auto get_root_directory(const char *path) -> String;
auto get_root_path(const char *path) -> String;
auto get_relative_path(const char *path) -> String;
auto get_parent_path(const char *path) -> String;
auto get_filename(const char *path) -> String;
auto get_filename_wo_ext(const char *path) -> String;
auto get_extension_w_dot(const char *path) -> String;

/// @brief contenate given paths with OS-based seperator character
/// @tparam ...P value types that can be allocated by String class
/// @param ...paths values that want to be concatenated
/// @return concatenated output string
template <typename... P> inline auto join(const P &...paths) -> String {
  const size_t total_paths = sizeof...(paths);
  std::array<absl::string_view, total_paths> values({paths...});

#if defined(__WIN32__) || defined(_WIN32)
  const absl::string_view path_sep = "\\";
#else
  const absl::string_view path_sep = "/";
#endif

  return absl::StrJoin(values, path_sep);
}
} // namespace path

} // namespace fs

} // namespace pxd