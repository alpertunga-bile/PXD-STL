#pragma once

#include "checks.hpp"

namespace pxd {
enum class ePXD_BT_ORDER : uint8_t {
  INORDER,
  PREORDER,
  POSTORDER,
};

template <typename T> class BinaryTree {
private:
  struct Node {
    T value;
    Node *left = nullptr;
    Node *right = nullptr;
  };

public:
  BinaryTree() = default;
  BinaryTree(const BinaryTree<T> &other) = delete;
  BinaryTree(BinaryTree<T> &&other) = delete;
  BinaryTree &operator=(const BinaryTree<T> &other) = delete;
  ~BinaryTree() { release(); }

  bool operator==(BinaryTree<T> &other) { return other.get_root() == root; }
  bool operator!=(BinaryTree<T> &other) { return other.get_root() != root; }

  void release(){};

  void add(T &value) {
    if (!IS_VALID(root)) {
      root = new Node();
      root->value = value;
      total_node_count++;
      return;
    }

    Node *current_node = root;
    Node *parent_node = nullptr;

    do {
      parent_node = current_node;
      if (value <= current_node->value) {
        current_node = current_node->left;
      } else {
        current_node = current_node->right;
      }
    } while (current_node != nullptr);

    current_node = new Node();
    current_node->value = value;

    PXD_ASSERT(parent_node != nullptr);

    if (value <= parent_node->value) {
      parent_node->left = current_node;
    } else {
      parent_node->right = current_node;
    }

    total_node_count++;
  }

  void add(T &&value) {
    if (!IS_VALID(root)) {
      root = new Node();
      root->value = value;
      total_node_count++;
      return;
    }

    Node *current_node = root;
    Node *parent_node = nullptr;

    do {
      parent_node = current_node;
      if (value <= current_node->value) {
        current_node = current_node->left;
      } else {
        current_node = current_node->right;
      }
    } while (current_node != nullptr);

    current_node = new Node();
    current_node->value = value;

    PXD_ASSERT(parent_node != nullptr);

    if (value <= parent_node->value) {
      parent_node->left = current_node;
    } else {
      parent_node->right = current_node;
    }

    total_node_count++;
  }

  void get_order(T *array, ePXD_BT_ORDER order) {
    if (root == nullptr) {
      return;
    }

    PXD_ASSERT(array != nullptr);
    int index = -1;

    switch (order) {
    case ePXD_BT_ORDER::INORDER:
      inorder(root, array, index);
      break;
    case ePXD_BT_ORDER::PREORDER:
      index = 0;
      preorder(root, array, index);
      break;
    case ePXD_BT_ORDER::POSTORDER:
      index = 0;
      postorder(root, array, index);
      break;

    default:
      break;
    }
  }

  inline Node *get_root() const { return root; }
  inline int get_total_node_count() { return total_node_count; }

private:
  void inorder(Node *node, T *array, int &index) {
    if (node == nullptr) {
      return;
    }

    inorder(node->left, array, index);
    index = index + 1;

    array[index] = node->value;

    inorder(node->right, array, index);
  }

  void preorder(Node *node, T *array, int &index) {
    if (node == nullptr) {
      return;
    }

    array[index] = node->value;
    index = index + 1;

    preorder(node->left, array, index);
    preorder(node->right, array, index);
  }

  void postorder(Node *node, T *array, int &index) {
    if (node == nullptr) {
      return;
    }

    postorder(node->left, array, index);
    postorder(node->right, array, index);

    array[index] = node->value;
    index = index + 1;
  }

private:
  Node *root = nullptr;
  int total_node_count = 0;
};
} // namespace pxd