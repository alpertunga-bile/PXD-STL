#pragma once

#include "checks.hpp"
#include "logger.hpp"

#include "../absl/flat_hash_map.hpp"
#include "double_linked_list.hpp"

namespace pxd {

template <typename Key, typename Val> class LRUCache {
private:
  struct LRUNode {
    Key key;
    Val value;

    inline auto operator==(const LRUNode &other) { return key == other.key; }
    inline auto operator==(LRUNode &&other) { return key == other.key; }
  };

public:
  LRUCache(size_t max_size = 32) : max_size(max_size) {};
  LRUCache(LRUCache &other) = default;
  auto operator=(LRUCache &other) -> LRUCache & = default;
  LRUCache(LRUCache &&other) = default;
  auto operator=(LRUCache &&other) -> LRUCache & = default;
  ~LRUCache() = default;

  void insert(Key key, Val value) {
    if (lru_map.contains(key)) {
      lru_map.erase(key);
    }

    LRUNode node;
    node.key = key;
    node.value = value;

    lru_list.add(node);
    lru_map.insert({key, node});

    if (lru_map.size() <= max_size) {
      return;
    }

    lru_list.remove(node);
    lru_map.erase(key);
  }

  auto get(Key key) -> Val {
    if (!lru_map.contains(key)) {
      PXD_LOG_WARNING("::PXD_LRU_CACHE:: Key is not exists")
      return Val();
    }

    LRUNode node = lru_map[key];
    lru_list.remove(node);
    lru_list.add(node);

    return node.value;
  }

private:
  size_t max_size;
  absl::flat_hash_map<Key, LRUNode> lru_map;
  DoubleLinkedList<LRUNode> lru_list;
};
} // namespace pxd