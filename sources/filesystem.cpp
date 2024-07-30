#include "filesystem.hpp"

#include <filesystem>

#include "string.hpp"

namespace pxd {
bool exists(const char *path) { return std::filesystem::exists(path); }

bool is_dir(const char *path) { return std::filesystem::is_directory(path); }

bool is_file(const char *path) {
  return std::filesystem::is_regular_file(path);
}

} // namespace pxd