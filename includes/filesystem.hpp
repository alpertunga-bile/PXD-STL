#pragma once

namespace pxd {
class String;
}

namespace pxd::fs {
bool exists(const char *path);
bool is_dir(const char *path);
bool is_file(const char *path);
void mkdir(const char *path);
size_t get_file_hash(const char *path);
String get_relative_path(const char *path);
String get_absolute_path(const char *path);
bool create_file_symlink(const char *filepath, const char *symlink);
bool create_directory_symlink(const char *dirpath, const char *symlink);
String getcwd();
String get_last_modified_time(const char *path);
bool remove_file(const char *path);
bool remove_folder(const char *path);
String get_temp_dir_path();
void copy_dir(const char *from, const char *to, bool is_recursive = true,
              bool update_existed = true);
void copy_file(const char *from, const char *to, bool update_existed = true);
void rename(const char *_old, const char *_new);
} // namespace pxd::fs

namespace pxd::fs::path {
String remove_filename(const char *path);
String replace_filename(const char *path, const char *new_filename);
String replace_extension(const char *path, const char *new_ext);
String get_root_name(const char *path);
String get_root_directory(const char *path);
String get_root_path(const char *path);
String get_relative_path(const char *path);
String get_parent_path(const char *path);
String get_filename(const char *path);
String get_filename_wo_ext(const char *path);
String get_extension_w_dot(const char *path);

/// @brief contenate given paths with OS-based seperator character
/// @tparam ...P value types that can be allocated by String class
/// @param ...paths values that want to be concatenated
/// @return concatenated output string
template <typename... P> inline String join(const P &...paths) {
  const String values[] = {paths...};
  const int n = sizeof...(paths) - 1;

#if defined(__WIN32__) || defined(_WIN32)
  const String path_sep = "\\";
#else
  const String path_sep = "/";
#endif

  String path = "";

  for (int i = 0; i < n; ++i) {
    path += (values[i] + path_sep);
  }

  path += values[n];

  return path;
}
} // namespace pxd::fs::path