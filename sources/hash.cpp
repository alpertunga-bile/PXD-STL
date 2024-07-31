#include "hash.hpp"

#include "filesystem.hpp"
#include "logger.hpp"
#include "string.hpp"

#include "core.h"

#include <fstream>
#include <sstream>

namespace pxd {
auto uint8_to_string(const std::array<uint8_t, BLAKE3_OUT_LEN> &computed_hashes)
    -> String {
  String str;

  for (auto &&hash : computed_hashes) {
    std::array<char, 3> temp;

    sprintf(temp.data(), "%02x", hash);

    str += temp.data();
  }

  return str;
}

auto pxd::comp_and_get_hash_str(const void *data,
                                size_t data_length) -> String {
  blake3_hasher hasher;
  blake3_hasher_init(&hasher);

  blake3_hasher_update(&hasher, data, data_length);

  std::array<uint8_t, BLAKE3_OUT_LEN> output;

  blake3_hasher_finalize(&hasher, output.data(), BLAKE3_OUT_LEN);

  return uint8_to_string(output);
}

void comp_hash(const void *data, size_t data_length,
               uint8_t *computed_hash_values) {
  blake3_hasher hasher;
  blake3_hasher_init(&hasher);

  blake3_hasher_update(&hasher, data, data_length);

  blake3_hasher_finalize(&hasher, computed_hash_values, BLAKE3_OUT_LEN);
}

auto get_hash_str(const std::array<uint8_t, BLAKE3_OUT_LEN> &hashed_values)
    -> String {
  return uint8_to_string(hashed_values);
}

auto get_file_content_hash_str(const char *filepath) -> String {
  String hash_str;

  if (!pxd::fs::exists(filepath)) {
    PXD_LOG_ERROR(fmt::format("{} is not exists", filepath).c_str());
    return hash_str;
  }

  std::ifstream file(filepath, std::ifstream::in);

  if (!file.good() || !file.is_open()) {
    PXD_LOG_ERROR(fmt::format("{} cannot opened", filepath).c_str());
    return hash_str;
  }

  std::stringstream contents;

  contents << file.rdbuf();

  file.close();

  hash_str = std::move(contents.str());

  return comp_and_get_hash_str(hash_str.c_str(), hash_str.length());
}

auto update_hasher_with_file_content(blake3_hasher *hasher,
                                     const char *filepath) -> bool {
  if (!pxd::fs::exists(filepath)) {
    PXD_LOG_ERROR(fmt::format("{} is not exists", filepath).c_str());
    return false;
  }

  std::ifstream file(filepath, std::ifstream::in);

  if (!file.good() || !file.is_open()) {
    PXD_LOG_ERROR(fmt::format("{} cannot opened", filepath).c_str());
    return false;
  }

  std::stringstream contents;

  contents << file.rdbuf();

  file.close();

  String hash_str = std::move(contents.str());

  blake3_hasher_update(hasher, hash_str.c_str(), hash_str.length());

  return true;
}

} // namespace pxd