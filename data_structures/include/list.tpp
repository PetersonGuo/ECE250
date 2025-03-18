/*
  Implementation of the List, Iterator, and ListNode class templates.
  This file contains the definitions of the List class template methods,
  which encapsulate a Linked List of values of potentially different types.
  It includes functionality for construction, assignment, comparison,
  and access to the List's elements.
*/

#ifndef LIST_TPP
#define LIST_TPP

#ifndef LIST_HPP
#error __FILE__ should only be included from list.hpp.
#endif // LIST_HPP

#include <cstdint>
#include <stdexcept>

// ListNode Implementation (Doubly Linked List Node)

template <typename T>
List<T>::ListNode::ListNode() : m_pNextNode(nullptr), m_pPrevNode(nullptr) {}

template <typename T>
List<T>::ListNode::ListNode(const T& data)
    : m_pNextNode(nullptr), m_pPrevNode(nullptr), m_data(data) {}

template <typename T>
List<T>::ListNode::ListNode(ListNode* next, const T& data)
    : m_pNextNode(next), m_pPrevNode(nullptr), m_data(data) {}

template <typename T>
List<T>::ListNode::ListNode(ListNode* prev, ListNode* next, const T& data)
    : m_pNextNode(next), m_pPrevNode(prev), m_data(data) {}

template <typename T>
void List<T>::ListNode::operator=(const T& data) {
  m_data = data;
}

template <typename T>
bool List<T>::ListNode::operator==(const T& data) const {
  return m_data == data;
}

template <typename T>
bool List<T>::ListNode::operator!=(const T& data) const {
  return m_data != data;
}

// List Implementation (Doubly Linked List)

template <typename T>
List<T>::List() : m_pHead(nullptr), m_pTail(nullptr), m_size(0) {}

template <typename T>
List<T>::~List() {
  clear();
}

template <typename T>
List<T>::List(const List<T>& other)
    : m_pHead(nullptr), m_pTail(nullptr), m_size(0) {
  for (ListNode* node = other.m_pHead; node != nullptr;
       node = node->m_pNextNode) {
    push_back(node->m_data);
  }
}

template <typename T>
List<T>& List<T>::operator=(const List<T>& other) {
  if (this != &other) {
    clear();
    for (ListNode* node = other.m_pHead; node != nullptr;
         node = node->m_pNextNode) {
      push_back(node->m_data);
    }
  }
  return *this;
}

template <typename T>
List<T>::List(List<T>&& other) noexcept
    : m_pHead(other.m_pHead), m_pTail(other.m_pTail), m_size(other.m_size) {
  other.m_pHead = other.m_pTail = nullptr;
  other.m_size = 0;
}

template <typename T>
List<T>& List<T>::operator=(List<T>&& other) noexcept {
  if (this != &other) {
    clear();
    m_pHead = other.m_pHead;
    m_pTail = other.m_pTail;
    m_size = other.m_size;
    other.m_pHead = other.m_pTail = nullptr;
    other.m_size = 0;
  }
  return *this;
}

template <typename T>
T List<T>::front() const {
  if (!m_pHead)
    throw std::runtime_error("List is empty");
  return m_pHead->m_data;
}

template <typename T>
T& List<T>::front() {
  if (!m_pHead)
    throw std::runtime_error("List is empty");
  return m_pHead->m_data;
}

template <typename T>
T List<T>::back() const {
  if (!m_pTail)
    throw std::runtime_error("List is empty");
  return m_pTail->m_data;
}

template <typename T>
T& List<T>::back() {
  if (!m_pTail)
    throw std::runtime_error("List is empty");
  return m_pTail->m_data;
}

template <typename T>
typename List<T>::Iterator List<T>::begin() const {
  return Iterator(m_pHead);
}

template <typename T>
typename List<T>::Iterator List<T>::begin() {
  return Iterator(m_pHead);
}

template <typename T>
typename List<T>::Iterator List<T>::rbegin() const {
  return Iterator(m_pTail);
}

template <typename T>
typename List<T>::Iterator List<T>::rbegin() {
  return Iterator(m_pTail);
}

template <typename T>
typename List<T>::Iterator List<T>::end() const {
  return Iterator(nullptr);
}

template <typename T>
typename List<T>::Iterator List<T>::end() {
  return List<T>::Iterator(nullptr);
}

template <typename T>
typename List<T>::Iterator List<T>::rend() const {
  return Iterator(nullptr);
}

template <typename T>
typename List<T>::Iterator List<T>::rend() {
  return List<T>::Iterator(nullptr);
}

template <typename T>
void List<T>::push_front(const T& data) {
  ListNode* temp = new ListNode(m_pHead, data);

  // If the list is empty, set both head and tail to the new node
  if (m_size == 0)
    m_pTail = temp;
  else
    m_pHead->m_pPrevNode = temp; // Link new node to the previous head

  m_pHead = temp; // Update the head pointer
  ++m_size;
}

template <typename T>
void List<T>::push_back(const T& data) {
  ListNode* temp = new ListNode(data);
  temp->m_pPrevNode = m_pTail; // Link new node to the previous tail

  // If the list is empty, set both head and tail to the new node
  if (m_size == 0)
    m_pHead = temp;
  else
    m_pTail->m_pNextNode = temp; // Update previous tail's next pointer

  m_pTail = temp; // Update tail pointer
  ++m_size;
}

template <typename T>
void List<T>::insert(const uint64_t index, const T& data) {
  if (index > m_size)
    throw std::out_of_range("Index out of bounds");

  if (index == 0) {
    push_front(data);
    return;
  }

  if (index == m_size) {
    push_back(data);
    return;
  }

  // Traverse to the insertion point
  ListNode* temp = m_pHead;
  for (uint64_t i = 0; i < index; ++i)
    temp = temp->m_pNextNode;

  // Insert new node between previous and current nodes
  ListNode* node = new ListNode(temp->m_pPrevNode, temp, data);
  temp->m_pPrevNode->m_pNextNode = node;
  temp->m_pPrevNode = node;
  ++m_size;
}

template <typename T>
T List<T>::pop_back() {
  if (!m_pTail)
    return T();

  ListNode* temp = m_pTail;
  T data = m_pTail->m_data;
  m_pTail = m_pTail->m_pPrevNode;

  // If there's still an element left, update tail's next pointer
  if (m_pTail)
    m_pTail->m_pNextNode = nullptr;

  delete temp;
  --m_size;

  return data;
}

template <typename T>
T List<T>::pop_front() {
  if (!m_pHead)
    return T();

  ListNode* temp = m_pHead;
  T data = m_pHead->m_data;
  m_pHead = m_pHead->m_pNextNode;

  // If there's still an element left, update head's prev pointer
  if (m_pHead)
    m_pHead->m_pPrevNode = nullptr;

  delete temp;
  --m_size;

  return data;
}

template <typename T>
void List<T>::erase(const uint64_t index) {
  if (index >= m_size)
    throw std::out_of_range("Index out of bounds");

  // Traverse to the node at the given index
  ListNode* temp = m_pHead;
  for (uint64_t i = 0; i < index; ++i)
    temp = temp->m_pNextNode;

  // Adjust pointers to remove node from list
  if (temp->m_pPrevNode)
    temp->m_pPrevNode->m_pNextNode = temp->m_pNextNode;
  if (temp->m_pNextNode)
    temp->m_pNextNode->m_pPrevNode = temp->m_pPrevNode;

  // Update head or tail pointers if necessary
  if (temp == m_pHead)
    m_pHead = temp->m_pNextNode;
  if (temp == m_pTail)
    m_pTail = temp->m_pPrevNode;

  delete temp;
  --m_size;
}

template <typename T>
void List<T>::erase(Iterator& it) {
  if (!it.m_node)
    return;

  ListNode* node = it.m_node;

  // Unlink from previous node if it exists; if not, update m_pHead.
  if (node->m_pPrevNode)
    node->m_pPrevNode->m_pNextNode = node->m_pNextNode;
  else
    m_pHead = node->m_pNextNode;

  // Unlink from next node if it exists; if not, update m_pTail.
  if (node->m_pNextNode)
    node->m_pNextNode->m_pPrevNode = node->m_pPrevNode;
  else
    m_pTail = node->m_pPrevNode;

  delete node;
  it.m_node = nullptr;
  --m_size;
}

template <typename T>
void List<T>::clear() {
  // Continuously remove front elements until the list is empty
  while (m_pHead)
    pop_front();
}

template <typename T>
typename List<T>::Iterator List<T>::find(const T& data) const {
  ListNode* temp = m_pHead;
  while (temp)
    if (temp->m_data == data)
      return Iterator(temp);
  return end();
}

template <typename T>
bool List<T>::empty() const {
  return m_size == 0;
}

template <typename T>
size_t List<T>::size() const {
  return m_size;
}

#endif // LIST_TPP