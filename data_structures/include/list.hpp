/*
  List class header file.
  This file defines a template-based doubly linked list implementation.
  It provides a List class for managing dynamic sequences of elements,
  along with a ListIterator for traversal and a ListNode for element storage.
*/

#ifndef LIST_HPP
#define LIST_HPP

#include <cstdint>
#include <cstring>

/*
  Template-based List class.
  Represents a dynamic doubly linked list.

  @tparam T Type of elements stored in the list.
*/
template <typename T>
class List {
private:
  /*
    Template-based ListNode class.
    Represents a single node in the doubly linked list.
    Each node stores data and pointers to the next and previous nodes.
  */
  class ListNode {
  private:
    ListNode *m_pNextNode, *m_pPrevNode; // Pointers to adjacent nodes.
    T m_data;                            // Data stored in the node.

  public:
    /**
     * @brief Default constructor.
     * Initializes an empty node with null next and previous pointers.
     */
    ListNode();

    /**
     * @brief Parameterized constructor.
     * Initializes a node with a given value.
     * @param data Value to store in the node.
     */
    ListNode(const T&);

    /**
     * @brief Constructor with next node.
     * Initializes a node and links it to a given next node.
     * @param next Pointer to the next node.
     * @param data Value to store in the node.
     */
    ListNode(ListNode*, const T&);

    /**
     * @brief Constructor with previous and next nodes.
     * Initializes a node and links it to given previous and next nodes.
     * @param prev Pointer to the previous node.
     * @param next Pointer to the next node.
     * @param data Value to store in the node.
     */
    ListNode(ListNode*, ListNode*, const T&);

    /**
     * @brief Assignment operator.
     * Assigns a new value to the node.
     * @param data Value to assign.
     */
    void operator=(const T&);

    /**
     * @brief Equality comparison operator.
     * Compares the node’s stored value with a given value.
     * @param data Value to compare with.
     * @return True if the values are equal, false otherwise.
     */
    bool operator==(const T&) const;

    /**
     * @brief Inequality comparison operator.
     * Checks if the stored value in the node is not equal to a given value.
     * @param data The value to compare with.
     * @return True if the stored value is not equal to the given value, false
     * otherwise.
     */
    bool operator!=(const T&) const;

    friend class List;
    friend class ListIterator;
  };

protected:
  ListNode *m_pHead, *m_pTail; // Pointers to the first and last nodes.
  size_t m_size;               // Current number of elements in the list.

public:
  /*
    Template-based ListIterator class.
    Provides an iterator for traversing a doubly linked list.
  */
  class ListIterator {
  private:
    ListNode* m_node; // Pointer to the current node.

  public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    /**
     * @brief Constructor for the iterator.
     * @param node Pointer to a ListNode to initialize the iterator.
     */
    explicit ListIterator(ListNode* node) : m_node(node) {}

    /**
     * @brief Dereference operator.
     * Accesses the value stored in the current node.
     * @return Reference to the stored value.
     */
    reference operator*() const {
      return m_node->m_data;
    }

    /**
     * @brief Arrow operator.
     * Provides access to the value stored in the current node.
     * @return Pointer to the stored value.
     */
    pointer operator->() const {
      return &(m_node->m_data);
    }

    /**
     * @brief Pre-increment operator.
     * Moves the iterator to the next node.
     * @return Reference to the updated iterator.
     */
    ListIterator& operator++() {
      if (m_node)
        m_node = m_node->m_pNextNode;
      return *this;
    }

    /**
     * @brief Post-increment operator.
     * Moves the iterator to the next node and returns the previous state.
     * @return Copy of the iterator before incrementing.
     */
    ListIterator operator++(int) {
      ListIterator temp = *this;
      ++(*this);
      return temp;
    }

    /**
     * @brief Pre-decrement operator.
     * Moves the iterator to the previous node.
     * @return Reference to the updated iterator.
     */
    ListIterator& operator--() {
      if (m_node)
        m_node = m_node->m_pPrevNode;
      return *this;
    }

    /**
     * @brief Post-decrement operator.
     * Moves the iterator to the previous node and returns the previous state.
     * @return Copy of the iterator before decrementing.
     */
    ListIterator operator--(int) {
      ListIterator temp = *this;
      --(*this);
      return temp;
    }

    /**
     * @brief Equality operator.
     * Checks if two iterators point to the same node.
     * @param other Another iterator to compare.
     * @return True if both iterators are equal.
     */
    bool operator==(const ListIterator& other) const {
      return m_node == other.m_node;
    }

    /**
     * @brief Inequality operator.
     * Checks if two iterators point to different nodes.
     * @param other Another iterator to compare.
     * @return True if the iterators are different.
     */
    bool operator!=(const ListIterator& other) const {
      return m_node != other.m_node;
    }

    friend class List;
  };

  /**
   * @brief Default constructor.
   * Initializes an empty list.
   */
  List();

  /**
   * @brief Destructor.
   * Clears the list and deallocates memory.
   */
  ~List();

  /**
   * @brief Copy constructor.
   * Creates a new list by copying all elements from another list.
   * @param other The List object to copy.
   */
  List(const List<T>& other);

  /**
   * @brief Copy assignment operator.
   * Copies all elements from another list into this list.
   * @param other The List object to copy.
   * @return Reference to the assigned object.
   */
  List<T>& operator=(const List<T>& other);

  /**
   * @brief Move constructor.
   * Transfers ownership of data from another list without copying.
   * @param other The List object to move.
   */
  List(List<T>&& other) noexcept;

  /**
   * @brief Move assignment operator.
   * Transfers ownership of data from another list, clearing the original.
   * @param other The List object to move.
   * @return Reference to the assigned object.
   */
  List<T>& operator=(List<T>&& other) noexcept;

  /**
   * @brief Returns a reference to the first element in the list.
   * @return Reference to the first element.
   * @throws std::runtime_error if the list is empty.
   */
  const T& front() const;
  T& front();

  /**
   * @brief Returns a reference to the last element in the list.
   * @return Reference to the last element.
   * @throws std::runtime_error if the list is empty.
   */
  const T& back() const;
  T& back();

  /**
   * @brief Returns an iterator pointing to the first element of the list.
   * @return A ListIterator to the first element.
   */
  ListIterator begin() const;
  ListIterator begin();

  /**
   * @brief Returns an iterator pointing to the last element of the list.
   * @return A ListIterator to the last element.
   */
  ListIterator rbegin() const;
  ListIterator rbegin();

  /**
   * @brief Returns an iterator pointing past the last element of the list.
   * Used to denote the end of the list in iteration.
   * @return A ListIterator pointing past the last element.
   */
  ListIterator end() const;
  ListIterator end();

  /**
   * @brief Returns an iterator pointing before the first element of the list.
   * Used to denote the reverse end of the list in iteration.
   * @return A ListIterator pointing before the first element.
   */
  ListIterator rend() const;
  ListIterator rend();

  /**
   * @brief Inserts an element at the front of the list.
   * @param data The value to insert.
   */
  void push_front(const T&);

  /**
   * @brief Inserts an element at the end of the list.
   * @param data The value to insert.
   */
  void push_back(const T&);

  /**
   * @brief Inserts an element at a specific index.
   * Shifts existing elements to accommodate the new value.
   * @param index Position where the value should be inserted.
   * @param data The value to insert.
   */
  void insert(const uint64_t, const T&);

  /**
   * @brief Removes the last element from the list.
   */
  void pop_back();

  /**
   * @brief Removes the first element from the list.
   */
  void pop_front();

  /**
   * @brief Removes an element at a specific index.
   * @param index Position of the element to remove.
   */
  void erase(const uint64_t);

  /**
   * @brief Removes an element at a specific index.
   * @param it Iterator pointing to element to remove.
   */
  void erase(const ListIterator&);

  /**
   * @brief Clears all elements from the list.
   */
  void clear();

  /**
   * @brief Finds an element in the list.
   * @param data The value to search for.
   * @return Iterator pointing to the element if found.
   */
  ListIterator find(const T& data);
};

#include "list.tpp"

#endif // LIST_HPP
