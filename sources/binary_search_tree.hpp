#pragma once

#include "checks.hpp"

namespace pxd {
enum class eBST_ORDER : uint8_t {
  INORDER,
  PREORDER,
  POSTORDER,
};

template <typename T> class Array;
template <typename T> class LinkedList;

template <typename T> class BinarySearchTree {
private:
  struct Node {
    T value;
    Node *left = nullptr;
    Node *right = nullptr;

    inline bool has_one_child() noexcept {
      return (left != nullptr && right == nullptr) ||
             (left == nullptr && right != nullptr);
    }

    inline bool has_two_children() noexcept {
      return left != nullptr && right != nullptr;
    }

    inline bool has_left() noexcept { return left != nullptr; }
    inline bool has_right() noexcept { return right != nullptr; }
  };

public:
  constexpr BinarySearchTree() noexcept = default;
  BinarySearchTree(T *values, int size, bool is_balance = false) {
    from_array(values, size, is_balance);
  }
  BinarySearchTree(Array<T> &array, bool is_balance = false) {
    from_array(array, is_balance);
  }
  BinarySearchTree(LinkedList<T> &linked_list, bool is_balance = false) {
    from_linked_list(linked_list, is_balance);
  }
  BinarySearchTree(const BinarySearchTree<T> &other) { from_bst(other); }
  constexpr BinarySearchTree(BinarySearchTree<T> &&other) noexcept {
    root = other.get_root();
    total_node_count = other.get_total_node_count();
    other.exec_move();
  }
  constexpr BinarySearchTree &operator=(BinarySearchTree<T> &&other) {
    release();

    root = other.get_root();
    total_node_count = other.get_total_node_count();
    other.exec_move();

    return *this;
  }
  BinarySearchTree &operator=(const BinarySearchTree<T> &other) {
    from_bst(other);
    return *this;
  }
  inline ~BinarySearchTree() noexcept { release(); }

  constexpr inline bool operator==(BinarySearchTree<T> &other) noexcept {
    return other.get_root() == root;
  }
  constexpr inline bool operator!=(BinarySearchTree<T> &other) noexcept {
    return other.get_root() != root;
  }

  inline void release() noexcept {
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

    if (is_contain(value)) {
      return;
    }

    Node *new_node = new Node();
    new_node->value = value;

    place_new_node(root, new_node);

    total_node_count++;
  }

  void add(T &&value) { add(value); }

  void from_array(T *array, int size, bool is_balance) {
    construct_from_array(array, size);

    if (is_balance) {
      balance_self();
    }
  }

  void from_array(Array<T> &array, bool is_balance) {
    if (array.get_length() == 0) {
      return;
    }

    construct_from_array(array.get_ptr(), array.get_length());

    if (is_balance) {
      balance_self();
    }
  }

  void from_linked_list(LinkedList<T> &linked_list, bool is_balance) {
    if (linked_list.get_length() == 0) {
      return 0;
    }

    release();

    const int size = linked_list.get_length();

    for (int i = 0; i < size; i++) {
      add(linked_list[i]);
    }

    if (is_balance) {
      balance_self();
    }
  }

  void remove(T &value) noexcept {
    if (root == nullptr) {
      return;
    }

    if (!is_contain(value)) {
      return;
    }

    Node *current_node = root;
    Node *parent_node = root;

    while (current_node->value != value) {
      parent_node = current_node;

      if (value < current_node->value) {
        current_node = current_node->left;
      } else if (value > current_node->value) {
        current_node = current_node->right;
      }
    }

    // when current node has one child
    remove_from_one_child(current_node, parent_node);

    // when current_node has two children
    remove_from_two_children(current_node, parent_node);

    // when current_node is root
    remove_from_root(current_node, parent_node);

    if (root == current_node) {
      delete root;
      total_node_count = 0;
      root = nullptr;
    } else {
      delete current_node;
      current_node = nullptr;

      total_node_count--;
    }
  }

  void remove(T &&value) noexcept { remove(value); }

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

  void get_order(Array<T> &array, eBST_ORDER &&order) const {
    if (root == nullptr) {
      return;
    }

    array.reallocate(total_node_count);
    int index = 0;

    switch (order) {
    case eBST_ORDER::INORDER:
      inorder(root, array.get_ptr(), index);
      break;
    case eBST_ORDER::PREORDER:
      preorder(root, array.get_ptr(), index);
      break;
    case eBST_ORDER::POSTORDER:
      postorder(root, array.get_ptr(), index);
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

  void balance_self() {
    if (root == nullptr && total_node_count < 3) {
      return;
    }

    int total_count = total_node_count;
    T *values = new T[total_node_count];
    get_order(values, eBST_ORDER::INORDER);

    release();

    build_balanced_tree_self(values, 0, total_count - 1);

    delete[] values;
  }

  bool is_contain(T &value) noexcept {
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

  constexpr bool is_contain(T &&value) noexcept { return is_contain(value); }

  T get_min_value() noexcept {
    if (root == nullptr) {
      return T();
    }

    Node *current_node = root;

    while (current_node->left != nullptr) {
      current_node = current_node->left;
    }

    return current_node->value;
  }

  T get_max_value() noexcept {
    if (root == nullptr) {
      return T();
    }

    Node *current_node = root;

    while (current_node->right != nullptr) {
      current_node = current_node->right;
    }

    return current_node->value;
  }

  inline Node *get_root() const noexcept { return root; }
  inline int get_total_node_count() const noexcept { return total_node_count; }
  inline void exec_move() noexcept {
    root = nullptr;
    total_node_count = 0;
  }

private:
  void inorder(Node *node, T *array, int &index) const noexcept {
    if (node == nullptr) {
      return;
    }

    inorder(node->left, array, index);

    array[index] = node->value;
    index = index + 1;

    inorder(node->right, array, index);
  }

  void preorder(Node *node, T *array, int &index) const noexcept {
    if (node == nullptr) {
      return;
    }

    array[index] = node->value;
    index = index + 1;

    preorder(node->left, array, index);
    preorder(node->right, array, index);
  }

  void postorder(Node *node, T *array, int &index) const noexcept {
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
    node = nullptr;
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
                           int end) noexcept {
    if (start > end) {
      return;
    }

    int mid = (start + end) / 2;
    bst.add(values[mid]);

    build_balanced_tree(bst, values, start, mid - 1);
    build_balanced_tree(bst, values, mid + 1, end);
  }

  void build_balanced_tree_self(T *values, int start, int end) noexcept {
    if (start > end) {
      return;
    }

    int mid = (start + end) / 2;
    add(values[mid]);

    build_balanced_tree_self(values, start, mid - 1);
    build_balanced_tree_self(values, mid + 1, end);
  }

  void place_new_node(Node *start, Node *new_node) noexcept {
    Node *current_node = start;
    Node *parent_node = nullptr;

    // to find the parent node
    while (current_node != nullptr) {
      parent_node = current_node;

      if (new_node->value < current_node->value) {
        current_node = current_node->left;
      } else if (new_node->value > current_node->value) {
        current_node = current_node->right;
      }
    }

    current_node = new_node;

    if (current_node->value < parent_node->value) {
      parent_node->left = current_node;
    } else {
      parent_node->right = current_node;
    }
  }

  void remove_from_one_child(Node *current_node, Node *parent_node) noexcept {
    if (current_node->has_one_child() && parent_node->right == current_node) {
      if (current_node->has_left()) {
        parent_node->right = current_node->left;
      } else {
        parent_node->right = current_node->right;
      }
    } else if (current_node->has_one_child() &&
               parent_node->left == current_node) {
      if (current_node->has_left()) {
        parent_node->left = current_node->left;
      } else {
        parent_node->left = current_node->right;
      }
    }
  }

  void remove_from_two_children(Node *current_node,
                                Node *parent_node) noexcept {
    if (current_node->has_two_children() && parent_node->left == current_node &&
        current_node->left->has_right()) {
      Node *new_node = current_node->left;
      place_new_node(current_node->right, new_node->right);
      new_node->right = current_node->right;
      parent_node->left = new_node;
    } else if (current_node->has_two_children() &&
               parent_node->right == current_node &&
               current_node->right->has_left()) {
      Node *new_node = current_node->right;
      place_new_node(current_node->left, new_node->left);
      new_node->left = current_node->left;
      parent_node->right = new_node;
    }
  }

  void remove_from_root(Node *current_node, Node *parent_node) noexcept {
    if (current_node == root && current_node->has_two_children()) {
      Node *new_node = current_node->right;
      place_new_node(new_node, current_node->left);
      root = new_node;
    } else if (current_node == root && current_node->has_one_child()) {
      if (current_node->has_left()) {
        root = current_node->left;
      } else {
        root = current_node->right;
      }
    }

    if (parent_node->left == current_node) {
      parent_node->left = nullptr;
    } else if (parent_node->right == current_node) {
      parent_node->right = nullptr;
    }
  }

private:
  Node *root = nullptr;
  int total_node_count = 0;
};
} // namespace pxd