#include "checks.hpp"

namespace pxd {

template<typename T>
struct TreapNode
{
  T key = T();
  double priority = 0;
  TreapNode<T>* left = nullptr;
  TreapNode<T>* right = nullptr;
  TreapNode<T>* parent = nullptr;

  TreapNode(T& _key, double _priority)
  {
    key = _key;
    priority = _priority;
  }

  void set_left(TreapNode<T>* node)
  {
    left = node;
    node->parent = this;
  }

  void set_right(TreapNode<T>* node)
  {
    right = node;
    node->parent = this;
  }
};

template<typename T>
class Treap
{
public:
  Treap() = default;
  Treap(const Treap<T>& other) = default;
  Treap& operator=(const Treap<T>& other) = default;
  Treap(Treap<T>&& other) = default;
  Treap& operator=(Treap<T>&& other) = default;
  inline ~Treap() noexcept {}

  void insert(T& key, double priority)
  {
    TreapNode<T>* node = root;
    TreapNode<T>* parent = nullptr;
    TreapNode<T>* new_node = new TreapNode<T>(key, priority);

    while (node != nullptr) {
      parent = node;

      if (node.key <= key) {
        node = node->left;
      }
    }
  }

  bool contains(T& key)
  {
    PXD_ASSERT(root != nullptr);

    return search_node(root, key) != nullptr ? true : false;
  }

private:
  // rotate left node to parent
  void right_rotate(TreapNode<T>* node)
  {
    PXD_ASSERT(node != nullptr && root != node)

    TreapNode<T>* node_parent = node->parent;

    PXD_ASSERT(node == node_parent->left);

    TreapNode<T>* node_upperparent = node_parent->parent;

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

  void left_rotate(TreapNode<T>* node)
  {
    PXD_ASSERT(node != nullptr && root != node)

    TreapNode<T>* node_parent = node->parent;

    PXD_ASSERT(node == node_parent->right);

    TreapNode<T>* node_upperparent = node_parent->parent;

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

  TreapNode<T>* search_node(TreapNode* node, T& key)
  {
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
  TreapNode<T>* root = nullptr;
};
}