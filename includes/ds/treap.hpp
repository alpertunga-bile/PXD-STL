#pragma once

#include "checks.hpp"

#include <limits>

namespace pxd {

template <typename T> struct TreapNode {
  T key = T();
  double priority = 0;
  TreapNode<T> *left = nullptr;
  TreapNode<T> *right = nullptr;
  TreapNode<T> *parent = nullptr;

  TreapNode(T &_key, double _priority) {
    key = _key;
    priority = _priority;
  }

  void set_left(TreapNode<T> *node) {
    left = node;
    node->parent = this;
  }

  void set_right(TreapNode<T> *node) {
    right = node;
    node->parent = this;
  }
};

template <typename T> class Treap {
public:
  Treap() = default;
  Treap(const Treap<T> &other) = default;
  Treap &operator=(const Treap<T> &other) = default;
  Treap(Treap<T> &&other) = default;
  Treap &operator=(Treap<T> &&other) = default;
  inline ~Treap() noexcept {}

  void insert(T &key, double priority) {
    TreapNode<T> *node = root;
    TreapNode<T> *parent = nullptr;
    TreapNode<T> *new_node = new TreapNode<T>(key, priority);

    while (node != nullptr) {
      parent = node;

      if (node.key <= key) {
        node = node->left;
      } else {
        node = node->right;
      }
    }

    if (parent == nullptr) {
      root = new_node;
      return;
    } else if (key <= parent->key) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }

    new_node->parent = parent;

    while (new_node->parent != nullptr &&
           new_node->priority < new_node->parent->priority) {
      if (new_node == new_node->parent->left) {
        right_rotate(new_node);
      } else {
        left_rotate(new_node);
      }
    }

    if (new_node->parent == nullptr) {
      root = new_node;
    }
  }

  void remove(const T &key) {
    TreapNode<T> *node = search_node(root, key);

    if (node == nullptr) {
      return;
    }

    if (root == node && node->left == nullptr && node->right == nullptr) {
      root = nullptr;
      return;
    }

    node->priority = std::numeric_limits<double>::max();

    while (node->left != nullptr || node->right != nullptr) {
      if (node->left != nullptr &&
          (node->right == nullptr ||
           node->left->priority > node->right->priority)) {
        right_rotate(node->left);
      } else {
        left_rotate(node->right);
      }

      if (node->parent == root) {
        root = node->parent;
      }
    }

    if (node->parent->left == node) {
      node->parent->left = nullptr;
    } else {
      node->parent->right = nullptr;
    }
  }

  T top() {
    PXD_ASSERT(root == nullptr);
    T key = root->key;

    remove(key);

    return key;
  }

  T peek() {
    PXD_ASSERT(root == nullptr);

    return root->key;
  }

  T min() {
    PXD_ASSERT(root == nullptr);

    TreapNode<T> *node = root;

    while (node->left != nullptr) {
      node = node->left;
    }

    return node->key;
  }

  T max() {
    PXD_ASSERT(root == nullptr);

    TreapNode<T> *node = root;

    while (node->right != nullptr) {
      node = node->right;
    }

    return node->key;
  }

  bool contains(const T &key) {
    PXD_ASSERT(root != nullptr);

    return search_node(root, key) != nullptr ? true : false;
  }

private:
  // rotate left node to parent
  void right_rotate(TreapNode<T> *node) {
    PXD_ASSERT(node != nullptr && root != node)

    TreapNode<T> *node_parent = node->parent;

    PXD_ASSERT(node == node_parent->left);

    TreapNode<T> *node_upperparent = node_parent->parent;

    if (node_upperparent == nullptr) {
      root == node;
    } else {
      if (node_upperparent->left == node_parent) {
        node_upperparent->set_left(node);
      } else {
        node_upperparent->set_right(node);
      }
    }

    node_parent->set_left(node->right);
    node->set_right(node_parent);
  }

  void left_rotate(TreapNode<T> *node) {
    PXD_ASSERT(node != nullptr && root != node)

    TreapNode<T> *node_parent = node->parent;

    PXD_ASSERT(node == node_parent->right);

    TreapNode<T> *node_upperparent = node_parent->parent;

    if (node_upperparent == nullptr) {
      root == node;
    } else {
      if (node_upperparent->left == node_parent) {
        node_upperparent->set_left(node);
      } else {
        node_upperparent->set_right(node);
      }
    }

    node_parent->set_right(node->left);
    node->set_left(node_parent);
  }

  TreapNode<T> *search_node(TreapNode *node, T &key) {
    if (node == nullptr) {
      return nullptr;
    }

    if (node.key == key) {
      return node;
    }

    if (node.key < key) {
      return search(node.left, key);
    } else {
      return search(node.right, key);
    }
  }

private:
  TreapNode<T> *root = nullptr;
};
} // namespace pxd