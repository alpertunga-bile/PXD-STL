#include "hash.hpp"

#include "string.hpp"

#include "blake3.h"

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

} // namespace pxd