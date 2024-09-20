#pragma once

#include "../hash.hpp"

#include "../absl/hash.hpp"

#include <array>
#include <math.h>
#include <string>

namespace pxd {

constexpr int BLOOM_FILTER_MAX_SIZE = 128;

template <int N = BLOOM_FILTER_MAX_SIZE> class BloomFilter {
public:
  BloomFilter() = default;
  BloomFilter(const BloomFilter &other) = default;
  auto operator=(const BloomFilter &other) -> BloomFilter & = default;
  BloomFilter(BloomFilter &&other) = default;
  auto operator=(BloomFilter &&other) -> BloomFilter & = default;
  ~BloomFilter() = default;

  template <typename T> void add(T value) {
    std::array<int, 3> indices;

    calc_hash_indices<T>(std::forward<T>(value), indices);

    filter[indices[0]] = true;
    filter[indices[1]] = true;
    filter[indices[2]] = true;
  }

  template <typename T> auto contains(T value) -> bool {
    std::array<int, 3> indices;

    calc_hash_indices<T>(std::forward<T>(value), indices);

    bool is_contains = true;

    is_contains = is_contains && filter[indices[0]];
    is_contains = is_contains && filter[indices[1]];
    is_contains = is_contains && filter[indices[2]];

    return is_contains;
  }

private:
  template <typename T>
  void calc_hash_indices(T &&value, std::array<int, 3> &indices) {
    auto absl_hash_value = absl::Hash<T>{}(value);
    auto std_hash_value = std::hash<T>{}(value);

    auto temp_str = std::to_string(absl_hash_value);
    uint8_t blake_comp_hashes[32];
    comp_hash(temp_str.c_str(), temp_str.length(), blake_comp_hashes);

    size_t blake_hash_value = 0;
    for (int i = 0; i < 32; ++i) {
      blake_hash_value += blake_comp_hashes[i] * std::pow(2, i);
    }

    indices[0] = absl_hash_value % N;
    indices[1] = std_hash_value % N;
    indices[2] = blake_hash_value % N;
  }

private:
  std::array<bool, N> filter = {false};
};
} // namespace pxd