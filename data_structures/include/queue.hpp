#ifndef QUEUE
#define QUEUE
#include "list.hpp"

/*
  Queue class.
  A template-based queue implementation that inherits from List.
  Provides standard queue operations: push, pop, peek, and front.
*/
template <typename T>
class Queue : private List<T> {
public:
  /**
   * @brief Checks if the queue is empty.
   * @return True if the queue is empty, false otherwise.
   */
  bool empty() {
    return !List<T>::m_pHead;
  }

  /**
   * @brief Removes and returns the front element of the queue.
   * @return The front element of the queue.
   * @throws std::runtime_error if the queue is empty.
   */
  T pop() {
    T front = List<T>::front();
    List<T>::pop_front();
    return front;
  }

  /**
   * @brief Returns the front element without removing it.
   * @return The front element of the queue.
   * @throws std::runtime_error if the queue is empty.
   */
  T peek() {
    return List<T>::front();
  }

  /**
   * @brief Inserts an element at the back of the queue.
   * @param data The element to be added to the queue.
   */
  void push(T data) {
    List<T>::push_back(data);
  }

  /**
   * @brief Returns the front element of the queue.
   * @return The front element of the queue.
   * @throws std::runtime_error if the queue is empty.
   */
  T front() {
    return List<T>::front();
  }
};

#endif // QUEUE
