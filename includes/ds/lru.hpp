#pragma once

#include "checks.hpp"
#include "logger.hpp"

#include "double_linked_list.hpp"

#include "../absl/flat_hash_map.hpp"

namespace pxd {

constexpr size_t PXD_LRU_CACHE_MAX_SIZE = 32;

template <typename Key, typename Val> class LRUCache {
private:
  struct LRUNode {
    Key key;
    Val value;

    auto operator==(const LRUNode &other) { return key == other.key; }
    auto operator==(LRUNode &&other) { return key == other.key; }
    auto operator!=(const LRUNode &other) { return key != other.key; }
    auto operator!=(LRUNode &&other) { return key != other.key; }
  };

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

    LRUNode node;
    node.key = key;
    node.value = value;

    lru_list.add(node);
    lru_map.insert(
        {key, std::make_unique<LRUNode>(lru_list.get_end_node()->value)});

    if (lru_map.size() <= max_size) {
      return;
    }

    auto head_node_value = lru_list.get_head_node()->value;

    lru_list.remove(head_node_value);
    lru_map.erase(head_node_value.key);
  }

  auto get(Key key) -> Val {
    if (!lru_map.contains(key)) {
      PXD_LOG_WARNING("::PXD_LRU_CACHE:: Key is not exists")
      return {};
    }

    LRUNode node = *(lru_map[key]);
    lru_list.remove(node);
    lru_list.add(node);

    return node.value;
  }

private:
  size_t max_size;
  DoubleLinkedList<LRUNode> lru_list;
  absl::flat_hash_map<Key, std::unique_ptr<LRUNode>> lru_map;
};
} // namespace pxd