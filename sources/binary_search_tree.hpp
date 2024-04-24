#pragma once

#include "checks.hpp"

namespace pxd {
enum class eBST_ORDER : uint8_t {
  INORDER,
  PREORDER,
  POSTORDER,
};

template <typename T> class BinarySearchTree {
private:
  struct Node {
    T value;
    Node *left = nullptr;
    Node *right = nullptr;
  };

public:
  BinarySearchTree() = default;
  BinarySearchTree(T *values, int size) { construct_from_array(values, size); }
  BinarySearchTree(const BinarySearchTree<T> &other) { from_bst(other); }
  BinarySearchTree(BinarySearchTree<T> &&other) { from_bst(other); }
  BinarySearchTree &operator=(const BinarySearchTree<T> &other) {
    from_bst(other);
    return *this;
  }
  ~BinarySearchTree() { release(); }

  bool operator==(BinarySearchTree<T> &other) {
    return other.get_root() == root;
  }
  bool operator!=(BinarySearchTree<T> &other) {
    return other.get_root() != root;
  }

  void release() {
    if (root == nullptr) {
      return;
    }

    release_tree(root);

    root = nullptr;
    total_node_count = 0;
  };

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
      if (value < current_node->value) {
        current_node = current_node->left;
      } else if (value > current_node->value) {
        current_node = current_node->right;
      } else {
        return;
      }
    } while (current_node != nullptr);

    current_node = new Node();
    current_node->value = value;

    PXD_ASSERT(parent_node != nullptr);

    if (value < parent_node->value) {
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
      if (value < current_node->value) {
        current_node = current_node->left;
      } else if (value > current_node->value) {
        current_node = current_node->right;
      } else {
        return;
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

  void get_order(T *array, eBST_ORDER &&order) const {
    if (root == nullptr) {
      return;
    }

    PXD_ASSERT(array != nullptr);
    int index = 0;

    switch (order) {
    case eBST_ORDER::INORDER:
      inorder(root, array, index);
      break;
    case eBST_ORDER::PREORDER:
      preorder(root, array, index);
      break;
    case eBST_ORDER::POSTORDER:
      postorder(root, array, index);
      break;

    default:
      break;
    }
  }

  BinarySearchTree<T> get_balanced_tree() {
    if (root == nullptr && total_node_count < 3) {
      return *this;
    }

    T *values = new T[total_node_count];
    get_order(values, eBST_ORDER::INORDER);

    BinarySearchTree<T> temp_bst;
    build_balanced_tree(temp_bst, values, 0, total_node_count - 1);

    delete[] values;

    return temp_bst;
  }

  bool is_contain(T &value) {
    if (root == nullptr) {
      return false;
    }

    Node *current_node = root;

    do {
      if (value < current_node->value) {
        current_node = current_node->left;
      } else if (value > current_node->value) {
        current_node = current_node->right;
      } else {
        return true;
      }
    } while (current_node != nullptr);

    return false;
  }

  bool is_contain(T &&value) {
    if (root == nullptr) {
      return false;
    }

    Node *current_node = root;

    do {
      if (value < current_node->value) {
        current_node = current_node->left;
      } else if (value > current_node->value) {
        current_node = current_node->right;
      } else {
        return true;
      }
    } while (current_node != nullptr);

    return false;
  }

  T get_min_value() {
    if (root == nullptr) {
      return T();
    }

    Node *current_node = root;

    while (current_node->left != nullptr) {
      current_node = current_node->left;
    }

    return current_node->value;
  }

  T get_max_value() {
    if (root == nullptr) {
      return T();
    }

    Node *current_node = root;

    while (current_node->right != nullptr) {
      current_node = current_node->right;
    }

    return current_node->value;
  }

  inline Node *get_root() const { return root; }
  inline int get_total_node_count() const { return total_node_count; }

private:
  void inorder(Node *node, T *array, int &index) const {
    if (node == nullptr) {
      return;
    }

    inorder(node->left, array, index);

    array[index] = node->value;
    index = index + 1;

    inorder(node->right, array, index);
  }

  void preorder(Node *node, T *array, int &index) const {
    if (node == nullptr) {
      return;
    }

    array[index] = node->value;
    index = index + 1;

    preorder(node->left, array, index);
    preorder(node->right, array, index);
  }

  void postorder(Node *node, T *array, int &index) const {
    if (node == nullptr) {
      return;
    }

    postorder(node->left, array, index);
    postorder(node->right, array, index);

    array[index] = node->value;
    index = index + 1;
  }

  void release_tree(Node *node) {
    if (node == nullptr) {
      return;
    }

    release_tree(node->left);
    release_tree(node->right);

    delete node;
  }

  void construct_from_array(T *array, int node_size) {
    release();

    for (int i = 0; i < node_size; i++) {
      add(array[i]);
    }
  }

  void from_bst(const BinarySearchTree<T> &other) {
    const int node_size = other.get_total_node_count();
    T *others_values = new T[node_size];

    other.get_order(others_values, eBST_ORDER::PREORDER);

    construct_from_array(others_values, node_size);

    delete[] others_values;
  }

  void from_bst(BinarySearchTree<T> &&other) {
    const int node_size = other.get_total_node_count();
    T *others_values = new T[node_size];

    other.get_order(others_values, eBST_ORDER::PREORDER);

    construct_from_array(others_values, node_size);

    delete[] others_values;
  }

  void build_balanced_tree(BinarySearchTree<T> &bst, T *values, int start,
                           int end) {
    if (start > end) {
      return;
    }

    int mid = (start + end) / 2;
    bst.add(values[mid]);

    build_balanced_tree(bst, values, start, mid - 1);
    build_balanced_tree(bst, values, mid + 1, end);
  }

private:
  Node *root = nullptr;
  int total_node_count = 0;
};
} // namespace pxd