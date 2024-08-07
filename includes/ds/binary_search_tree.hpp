#pragma once

#include "../checks.hpp"
#include <cstdint>

namespace pxd {
enum class eBST_ORDER : std::uint8_t {
  INORDER,
  PREORDER,
  POSTORDER,
};

template <typename T> struct BSTNode {
  T value;
  BSTNode<T> *left = nullptr;
  BSTNode<T> *right = nullptr;

  auto has_one_child() noexcept -> bool {
    return (left != nullptr && right == nullptr) ||
           (left == nullptr && right != nullptr);
  }

  auto has_two_children() noexcept -> bool {
    return left != nullptr && right != nullptr;
  }

  auto has_left() noexcept -> bool { return left != nullptr; }
  auto has_right() noexcept -> bool { return right != nullptr; }
};

template <typename T> class BinarySearchTree {
public:
  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Constructors

  constexpr BinarySearchTree() noexcept = default;
  BinarySearchTree(T *values, int size, bool is_balance = false) {
    from_array(values, size, is_balance);
  }
  BinarySearchTree(const BinarySearchTree<T> &other) { from_bst(other); }
  constexpr BinarySearchTree(BinarySearchTree<T> &&other) noexcept
      : root(other.get_root()), total_node_count(other.get_total_node_count()) {
    other.exec_move();
  }
  constexpr auto operator=(BinarySearchTree<T> &&other) -> BinarySearchTree & {
    release();

    root = other.get_root();
    total_node_count = other.get_total_node_count();
    other.exec_move();

    return *this;
  }
  auto operator=(const BinarySearchTree<T> &other) -> BinarySearchTree & {
    if (other.get_root() == root) {
      return *this;
    }

    from_bst(other);
    return *this;
  }
  ~BinarySearchTree() noexcept { release(); }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Operator Overloads

  constexpr auto operator==(BinarySearchTree<T> &other) noexcept -> bool {
    return other.get_root() == root;
  }
  constexpr auto operator!=(BinarySearchTree<T> &other) noexcept -> bool {
    return other.get_root() != root;
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // DS Functionalities

  void release() noexcept {
    if (root == nullptr) {
      return;
    }

    release_tree(root);

    root = nullptr;
    total_node_count = 0;
  };

  void add(const T &value) {
    if (!IS_VALID_PTR(root)) {
      root = new BSTNode<T>();
      root->value = value;
      total_node_count++;
      return;
    }

    if (is_contain(value)) {
      return;
    }

    BSTNode<T> *new_node = new BSTNode<T>();
    new_node->value = value;

    place_new_node(root, new_node);

    total_node_count++;
  }

  void add(T &&value) { add(value); }

  void remove(const T &value) noexcept {
    if (root == nullptr) {
      return;
    }

    if (!is_contain(value)) {
      return;
    }

    BSTNode<T> *current_node = root;
    BSTNode<T> *parent_node = root;

    while (current_node->value != value) {
      parent_node = current_node;

      if (value < current_node->value) {
        current_node = current_node->left;
      } else if (value > current_node->value) {
        current_node = current_node->right;
      }
    }

    if (root == current_node) {
      delete root;
      root = nullptr;

      total_node_count = 0;

      return;
    }

    if (current_node->has_one_child()) {
      remove_from_one_child(current_node, parent_node);
    } else if (current_node->has_two_children()) {
      remove_from_two_children(current_node, parent_node);
    } else if (current_node == root) {
      remove_from_root(current_node, parent_node);
    }

    delete current_node;
    current_node = nullptr;

    total_node_count--;
  }

  void remove(T &&value) noexcept { remove(value); }

  auto get_balanced_tree() -> BinarySearchTree<T> {
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

  auto is_contain(const T &value) noexcept -> bool {
    if (root == nullptr) {
      return false;
    }

    BSTNode<T> *current_node = root;

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

  constexpr auto is_contain(T &&value) noexcept -> bool {
    return is_contain(value);
  }

  auto get_min_value() noexcept -> T {
    if (root == nullptr) {
      return T();
    }

    BSTNode<T> *current_node = root;

    while (current_node->left != nullptr) {
      current_node = current_node->left;
    }

    return current_node->value;
  }

  auto get_max_value() noexcept -> T {
    if (root == nullptr) {
      return T();
    }

    BSTNode<T> *current_node = root;

    while (current_node->right != nullptr) {
      current_node = current_node->right;
    }

    return current_node->value;
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // From Functions

  void from_array(T *array, int size, bool is_balance) {
    construct_from_array(array, size);

    if (is_balance) {
      balance_self();
    }
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // To Functions

  void get_order(T *array, eBST_ORDER order) const {
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

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Inline Member Funcs

  auto get_root() const noexcept -> BSTNode<T> * { return root; }
  auto get_total_node_count() const noexcept -> int { return total_node_count; }
  constexpr void exec_move() noexcept {
    root = nullptr;
    total_node_count = 0;
  }

private:
  void inorder(BSTNode<T> *node, T *array, int &index) const noexcept {
    if (node == nullptr) {
      return;
    }

    inorder(node->left, array, index);

    array[index] = node->value;
    index = index + 1;

    inorder(node->right, array, index);
  }

  void preorder(BSTNode<T> *node, T *array, int &index) const noexcept {
    if (node == nullptr) {
      return;
    }

    array[index] = node->value;
    index = index + 1;

    preorder(node->left, array, index);
    preorder(node->right, array, index);
  }

  void postorder(BSTNode<T> *node, T *array, int &index) const noexcept {
    if (node == nullptr) {
      return;
    }

    postorder(node->left, array, index);
    postorder(node->right, array, index);

    array[index] = node->value;
    index = index + 1;
  }

  void release_tree(BSTNode<T> *node) {
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

  void place_new_node(BSTNode<T> *start, BSTNode<T> *new_node) noexcept {
    BSTNode<T> *current_node = start;
    BSTNode<T> *parent_node = nullptr;

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

  void remove_from_one_child(BSTNode<T> *current_node,
                             BSTNode<T> *parent_node) noexcept {
    if (parent_node->right == current_node) {
      if (current_node->has_left()) {
        parent_node->right = current_node->left;
      } else {
        parent_node->right = current_node->right;
      }
    } else if (parent_node->left == current_node) {
      if (current_node->has_left()) {
        parent_node->left = current_node->left;
      } else {
        parent_node->left = current_node->right;
      }
    }
  }

  void remove_from_two_children(BSTNode<T> *current_node,
                                BSTNode<T> *parent_node) noexcept {
    if (parent_node->left == current_node && current_node->left->has_right()) {
      BSTNode<T> *new_node = current_node->left;
      place_new_node(current_node->right, new_node->right);
      new_node->right = current_node->right;
      parent_node->left = new_node;
    } else if (parent_node->right == current_node &&
               current_node->right->has_left()) {
      BSTNode<T> *new_node = current_node->right;
      place_new_node(current_node->left, new_node->left);
      new_node->left = current_node->left;
      parent_node->right = new_node;
    }
  }

  void remove_from_root(BSTNode<T> *current_node,
                        BSTNode<T> *parent_node) noexcept {
    if (current_node->has_two_children()) {
      BSTNode<T> *new_node = current_node->right;
      place_new_node(new_node, current_node->left);
      root = new_node;
    } else if (current_node->has_one_child()) {
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
  BSTNode<T> *root = nullptr;
  int total_node_count = 0;
};
} // namespace pxd