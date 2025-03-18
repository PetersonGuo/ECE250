#ifndef STACK
#define STACK
#include "list.hpp"

/*
  Stack class.
  A template-based stack implementation that inherits from List.
  Provides standard stack operations: push, pop, top, and empty check.
*/
template <typename T>
class Stack : private List<T> {
public:
  /**
   * @brief Checks if the stack is empty.
   * @return True if the stack is empty, false otherwise.
   */
  bool empty() {
    return !List<T>::m_pHead;
  }

  /**
   * @brief Removes and returns the top element of the stack.
   * @return The top element of the stack.
   * @throws std::runtime_error if the stack is empty.
   */
  T pop() {
    T front = List<T>::front();
    List<T>::pop_front();
    return front;
  }

  /**
   * @brief Pushes an element onto the top of the stack.
   * @param data The element to be added to the stack.
   */
  void push(T data) {
    List<T>::push_front(data);
  }

  /**
   * @brief Returns the top element of the stack without removing it.
   * @return The top element of the stack.
   * @throws std::runtime_error if the stack is empty.
   */
  T top() {
    return List<T>::front();
  }
};

#endif // STACK
