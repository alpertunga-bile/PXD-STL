#pragma once

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
String get_hash_str(const uint8_t *hashed_values, size_t length = 32);

} // namespace pxd