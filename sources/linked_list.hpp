#pragma once

namespace pxd {
template<typename T>
class LinkedList
{
private:
  struct Node
  {
    T value;
    Node* next = nullptr;
  };

public:
  LinkedList()
  {
    head = nullptr;
    end = nullptr;
    length = 0;
  } // default constructor
  LinkedList(T* node_list) {}

  LinkedList(const LinkedList<T>& other) = delete;
  LinkedList(LinkedList<T>&& other) = delete;
  LinkedList& operator=(const LinkedList<T>& other) = delete;

  ~LinkedList()
  {
    if (head == nullptr) {
      return;
    }

    Node* current_node = head;

    do {
      Node* prev_node = current_node;
      current_node = current_node->next;
      delete prev_node
    } while (current_node != nullptr);

    head = nullptr;
    end = nullptr;
  }

  void add(T& new_value)
  {
    Node* new_node = new Node();
    new_node->value = new_value;

    if (head == nullptr) {
      head = new_node;
    } else {
      head->next = new_node;
    }

    end = new_node;
    length += 1;
  }

  inline int get_length() { return length; }

private:
  Node* head = nullptr;
  Node* end = nullptr;
  int length = 0;
};
}