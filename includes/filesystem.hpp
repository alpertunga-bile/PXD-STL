#pragma once

namespace pxd {

class String;

inline bool exists(const char *path);
inline bool is_dir(const char *path);
inline bool is_file(const char *path);

template <typename... P> inline String join_paths(const P &...paths) {
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
} // namespace pxd