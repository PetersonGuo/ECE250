#include "list.hpp"
#include <cstdint>
#include <stdexcept>

// ListNode Implementation (Doubly Linked List Node)

template <typename T>
List<T>::ListNode::ListNode() : m_pNextNode(nullptr), m_pPrevNode(nullptr)
{}

template <typename T>
List<T>::ListNode::ListNode(const T& data)
    : m_pNextNode(nullptr), m_pPrevNode(nullptr), m_data(data)
{}

template <typename T>
List<T>::ListNode::ListNode(ListNode* next, const T& data)
    : m_pNextNode(next), m_pPrevNode(nullptr), m_data(data)
{}

template <typename T>
List<T>::ListNode::ListNode(ListNode* prev, ListNode* next, const T& data)
    : m_pNextNode(next), m_pPrevNode(prev), m_data(data)
{}

template <typename T>
void List<T>::ListNode::operator=(const T& data)
{
  m_data = data;
}

template <typename T>
bool List<T>::ListNode::operator==(const T& data) const
{
  return m_data == data;
}

template <typename T>
typename List<T>::ListNode*& List<T>::ListNode::next()
{
  return m_pNextNode;
}

template <typename T>
typename List<T>::ListNode*& List<T>::ListNode::prev()
{
  return m_pPrevNode;
}

template <typename T>
T& List<T>::ListNode::getData()
{
  return m_data;
}

// List Implementation (Doubly Linked List)

template <typename T>
List<T>::List() : m_pHead(nullptr), m_pTail(nullptr), m_size(0)
{}

template <typename T>
List<T>::~List()
{
  clear();
}

template <typename T>
const T& List<T>::front() const
{
  if (!m_pHead)
    throw std::runtime_error("List is empty");
  return m_pHead->getData();
}

template <typename T>
T& List<T>::front()
{
  if (!m_pHead)
    throw std::runtime_error("List is empty");
  return m_pHead->getData();
}

template <typename T>
const T& List<T>::back() const
{
  if (!m_pTail)
    throw std::runtime_error("List is empty");
  return m_pTail->getData();
}

template <typename T>
T& List<T>::back()
{
  if (!m_pTail)
    throw std::runtime_error("List is empty");
  return m_pTail->getData();
}

template <typename T>
typename List<T>::ListIterator List<T>::begin() const
{
  return ListIterator(m_pHead);
}

template <typename T>
typename List<T>::ListIterator List<T>::begin()
{
  return ListIterator(m_pHead);
}

template <typename T>
typename List<T>::ListIterator List<T>::rbegin() const
{
  return ListIterator(m_pTail);
}

template <typename T>
typename List<T>::ListIterator List<T>::rbegin()
{
  return ListIterator(m_pTail);
}

template <typename T>
typename List<T>::ListIterator List<T>::end() const
{
  return ListIterator(nullptr);
}

template <typename T>
typename List<T>::ListIterator List<T>::end()
{
  return List<T>::ListIterator(nullptr);
}

template <typename T>
typename List<T>::ListIterator List<T>::rend() const
{
  return ListIterator(nullptr);
}

template <typename T>
typename List<T>::ListIterator List<T>::rend()
{
  return List<T>::ListIterator(nullptr);
}

template <typename T>
void List<T>::push_front(const T& data)
{
  ListNode* temp = new ListNode(m_pHead, data);

  // If the list is empty, set both head and tail to the new node
  if (m_size == 0)
    m_pTail = temp;
  else
    m_pHead->prev() = temp; // Link new node to the previous head

  m_pHead = temp; // Update the head pointer
  ++m_size;
}

template <typename T>
void List<T>::push_back(const T& data)
{
  ListNode* temp = new ListNode(data);
  temp->prev() = m_pTail; // Link new node to the previous tail

  // If the list is empty, set both head and tail to the new node
  if (m_size == 0)
    m_pHead = temp;
  else
    m_pTail->next() = temp; // Update previous tail's next pointer

  m_pTail = temp; // Update tail pointer
  ++m_size;
}

template <typename T>
void List<T>::insert(const uint64_t index, const T& data)
{
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
    temp = temp->next();

  // Insert new node between previous and current nodes
  ListNode* node = new ListNode(temp->prev(), temp, data);
  temp->prev()->next() = node;
  temp->prev() = node;
  ++m_size;
}

template <typename T>
void List<T>::pop_back()
{
  if (!m_pTail)
    return;

  ListNode* temp = m_pTail;
  m_pTail = m_pTail->prev();

  // If there's still an element left, update tail's next pointer
  if (m_pTail)
    m_pTail->next() = nullptr;

  delete temp;
  --m_size;
}

template <typename T>
void List<T>::pop_front()
{
  if (!m_pHead)
    return;

  ListNode* temp = m_pHead;
  m_pHead = m_pHead->next();

  // If there's still an element left, update head's prev pointer
  if (m_pHead)
    m_pHead->prev() = nullptr;

  delete temp;
  --m_size;
}

template <typename T>
void List<T>::erase(const uint64_t index)
{
  if (index >= m_size)
    throw std::out_of_range("Index out of bounds");

  // Traverse to the node at the given index
  ListNode* temp = m_pHead;
  for (uint64_t i = 0; i < index; ++i)
    temp = temp->next();

  // Adjust pointers to remove node from list
  if (temp->prev())
    temp->prev()->next() = temp->next();
  if (temp->next())
    temp->next()->prev() = temp->prev();

  // Update head or tail pointers if necessary
  if (temp == m_pHead)
    m_pHead = temp->next();
  if (temp == m_pTail)
    m_pTail = temp->prev();

  delete temp;
  --m_size;
}

template <typename T>
void List<T>::erase(const T& data)
{
  ListNode* temp = m_pHead;
  uint64_t i = 0;

  // Find the first occurrence of the data
  while (temp && temp->getData() != data) {
    temp = temp->next();
    ++i;
  }

  // If data is found, remove it
  if (temp)
    erase(i);
}

template <typename T>
void List<T>::clear()
{
  // Continuously remove front elements until the list is empty
  while (m_pHead)
    pop_front();
}
