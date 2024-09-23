#pragma once

#include "checks.hpp"
#include "logger.hpp"

#include "double_linked_list.hpp"

#include "../absl/flat_hash_map.hpp"

namespace pxd {

constexpr size_t PXD_LRU_CACHE_MAX_SIZE = 32;

template <typename Key, typename Val> class LRUCache {
public:
  LRUCache(size_t max_size = PXD_LRU_CACHE_MAX_SIZE) : max_size(max_size) {};
  LRUCache(const LRUCache &other) = default;
  auto operator=(const LRUCache &other) -> LRUCache & = default;
  LRUCache(LRUCache &&other) = default;
  auto operator=(LRUCache &&other) -> LRUCache & = default;
  ~LRUCache() = default;

  void insert(Key key, Val value) {
    if (lru_map.contains(key)) {
      lru_map.erase(key);
    }

    lru_list.add(value);
    lru_map.insert(
        {key, std::make_unique<Val>(lru_list.get_end_node()->value)});

    if (lru_map.size() <= max_size) {
      return;
    }

    lru_list.remove(value);
    lru_map.erase(key);
  }

  auto get(Key key) -> Val {
    if (!lru_map.contains(key)) {
      PXD_LOG_WARNING("::PXD_LRU_CACHE:: Key is not exists")
      return {};
    }

    Val value = *(lru_map[key]);
    lru_list.remove(value);
    lru_list.add(value);

    return value;
  }

private:
  size_t max_size;
  DoubleLinkedList<Val> lru_list;
  absl::flat_hash_map<Key, std::unique_ptr<Val>> lru_map;
};
} // namespace pxd