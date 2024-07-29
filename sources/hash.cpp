#include "hash.hpp"

#include "logger.hpp"
#include "string.hpp"

#include "core.h"

#include <filesystem>
#include <fstream>

namespace pxd {
String uint8_to_string(const uint8_t *computed_hash, const size_t size) {
  String str;

  for (int i = 0; i < size; ++i) {
    char temp[3];
    sprintf_s(temp, "%02x", computed_hash[i]);

    str += temp;
  }

  return str;
}

String pxd::comp_and_get_hash_str(const void *data, size_t length) {
  blake3_hasher hasher;
  blake3_hasher_init(&hasher);

  blake3_hasher_update(&hasher, data, length);

  uint8_t output[BLAKE3_OUT_LEN];

  blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);

  return uint8_to_string(output, BLAKE3_OUT_LEN);
}

void comp_hash(const void *data, size_t length, uint8_t *computed_hash_values) {
  blake3_hasher hasher;
  blake3_hasher_init(&hasher);

  blake3_hasher_update(&hasher, data, length);

  blake3_hasher_finalize(&hasher, computed_hash_values, BLAKE3_OUT_LEN);
}

String get_hash_str(const uint8_t *hashed_values, size_t length) {
  return uint8_to_string(hashed_values, length);
}

String get_file_content_hash_str(const char *filepath) {
  String hash_str;

  if (!std::filesystem::exists(filepath)) {
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

bool update_hasher_with_file_content(blake3_hasher *hasher,
                                     const char *filepath) {
  if (!std::filesystem::exists(filepath)) {
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