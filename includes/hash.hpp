#pragma once

#include "blake3.h"

#include <cstdint>

namespace pxd {
class String;

/// @brief compute the hash value of the data and store it in the
/// computed_hash_values
/// @param data given input values
/// @param length length of the data value
/// @param computed_hash_values output of the hash of the data value
void comp_hash(const void *data, size_t length, uint8_t *computed_hash_values);

/// @brief compute the hash value of the data and get string value of the hash
/// @param data given input values
/// @param length length of the data value
/// @return string value of the hash based on hex
String comp_and_get_hash_str(const void *data, size_t length);

/// @brief get the string value from the computed hashed values
/// @param hashed_values precomputed hash values
/// @param length length of the hashed_values, default to 32
/// @return string value of the hash based on hex
String get_hash_str(const uint8_t *hashed_values,
                    size_t length = BLAKE3_OUT_LEN);

/// @brief get the hash string from the file contents
/// @param filepath filepath to the exists file
/// @return computed hash string of the file contents
String get_file_content_hash_str(const char *filepath);

bool update_hasher_with_file_content(blake3_hasher *hasher,
                                     const char *filepath);

template <typename... V>
void join_and_comp_hash(uint8_t *computed_hash_values, const V &...strings) {
  const String values[] = {strings...};
  const int n = sizeof...(strings);

  blake3_hasher hasher;
  blake3_hasher_init(&hasher);

  for (int i = 0; i < n; ++i) {
    const bool is_file_update =
        update_hasher_with_file_content(hasher, values[i].c_str());

    if (is_file_update) {
      continue;
    }

    blake3_hasher_update(&hasher, values[i].c_str(), values[i].length());
  }

  blake3_hasher_finalize(&hasher, computed_hash_values, BLAKE3_OUT_LEN);
}

} // namespace pxd