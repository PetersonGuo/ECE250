/*
  Implementation of the Vector class template.
  This file contains the definitions of the Vector class template methods,
  which encapsulate a dynamic array of values of potentially different types.
  It includes functionality for construction, assignment, comparison,
  and access to the Vector's elements.
*/
#ifndef VECTOR_TPP
#define VECTOR_TPP

#ifndef VECTOR_HPP
#error __FILE__ should only be included from vector.hpp.
#endif // VECTOR_HPP

#include <stdexcept>
#include <new>
#include <utility>

template <typename T>
void Vector<T>::reallocate() {
  // Ensure the vector has a minimum capacity of 2
  if (m_capacity < 2) {
    resize(2);
  }
  // Double the capacity when the vector is full
  else if (m_size >= m_capacity) {
    m_capacity =
        m_size << 1; // Bitwise left shift is equivalent to multiplying by 2
    resize(m_capacity);
  }
  // Reduce capacity by half if size is less than a quarter of capacity
  else if (m_capacity > 2 && m_size * 4 <= m_capacity) {
    m_capacity >>= 1; // Bitwise right shift is equivalent to dividing by 2
    resize(m_capacity < 2 ? 2 : m_capacity);
  }
}

template <typename T>
Vector<T>::Vector(const size_t size) : m_size(size), m_pValues(nullptr) {
  // Ensure the minimum capacity is 2
  m_capacity = size < 2 ? 2 : size;
  resize(m_capacity);
}

template <typename T>
Vector<T>::Vector(const Vector& other)
    : m_capacity(other.m_capacity), m_size(other.m_size), m_pValues(nullptr) {
  resize(m_capacity);

  // Copy all elements from the other vector
  for (uint64_t i = 0; i < m_size; ++i)
    m_pValues[i] = other.m_pValues[i];
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
  if (this != &other) { // Prevent self-assignment
    close();            // Free existing memory

    m_size = other.m_size;
    m_capacity = other.m_capacity;
    resize(m_capacity);

    // Copy elements from the other vector
    for (uint64_t i = 0; i < m_size; ++i)
      m_pValues[i] = other.m_pValues[i];
  }
  return *this;
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept {
  close();

  // Transfer ownership of the data
  m_pValues = other.m_pValues;
  other.m_pValues = nullptr;

  m_size = other.m_size;
  m_capacity = other.m_capacity;

  other.close();
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
  if (this != &other) {
    close();

    // Transfer ownership of the data
    m_pValues = other.m_pValues;
    other.m_pValues = nullptr;

    m_size = other.m_size;
    m_capacity = other.m_capacity;

    other.close();
  }
  return *this;
}

template <typename T>
Vector<T>::~Vector() {
  close();
}

template <typename T>
T& Vector<T>::operator[](const uint64_t index) {
  if (index >= m_size)
    throw std::out_of_range("Index out of bounds");

  return m_pValues[index];
}

template <typename T>
const T& Vector<T>::operator[](const uint64_t index) const {
  if (index >= m_size)
    throw std::out_of_range("Index out of bounds");

  return m_pValues[index];
}

template <typename T>
T* Vector<T>::begin() {
  return m_pValues;
}
template <typename T>
const T* Vector<T>::begin() const {
  return m_pValues;
}

template <typename T>
T* Vector<T>::end() {
  return m_pValues + m_size; // Pointer arithmetic: one past the last element
}
template <typename T>
const T* Vector<T>::end() const {
  return m_pValues + m_size; // Pointer arithmetic: one past the last element
}

template <typename T>
T& Vector<T>::front() {
  return *m_pValues; // Dereference the pointer to get the first element
}

template <typename T>
T& Vector<T>::back() {
  return m_pValues[m_size - 1]; // Access the last element using index
}

template <typename T>
void Vector<T>::resize(const size_t capacity) {
  T* temp = new T[capacity]; // Allocate new memory block
  if (!temp)
    throw std::bad_alloc();

  if (m_pValues) {
    // Move existing elements to new memory block
    for (size_t i = 0; i < m_size; ++i)
      temp[i] = std::move(m_pValues[i]);
    delete[] m_pValues; // Free old memory
  }

  m_pValues = temp;
  m_capacity = capacity;
}

template <typename T>
void Vector<T>::insert(const uint64_t index, const T& value) {
  if (index > m_size)
    throw std::out_of_range("Insertion out of bounds");

  reallocate();

  // Shift elements to the right to make space for the new element
  for (size_t i = m_size; i > index; --i)
    m_pValues[i] = std::move(m_pValues[i - 1]);

  m_pValues[index] = value;
  ++m_size;
}

template <typename T>
template <class... Args>
void Vector<T>::emplace(const uint64_t index, Args&&... args) {
  if (index > m_size)
    throw std::out_of_range("Insertion out of bounds");

  reallocate();

  // Shift elements to the right to make space for the new element
  for (size_t i = m_size; i > index; --i)
    m_pValues[i] = std::move(m_pValues[i - 1]);

  // Construct the new element in place using perfect forwarding
  m_pValues[index] = T(std::forward<Args>(args)...);
  ++m_size;
}

template <typename T>
void Vector<T>::push_back(const T& value) {
  reallocate();
  m_pValues[m_size++] = value; // Append value at the next available position
}

template <typename T>
template <class... Args>
void Vector<T>::emplace_back(Args&&... args) {
  reallocate();
  m_pValues[m_size++] = T(std::forward<Args>(args)...); // Construct in place
}

template <typename T>
void Vector<T>::pop_back() {
  if (m_size == 0)
    throw std::out_of_range("Cannot pop_back from an empty vector");

  reallocate();
  --m_size;
}

template <typename T>
void Vector<T>::clear() {
  m_capacity = 2; // Reset to minimum capacity
  m_size = 0;
  resize(m_capacity);
}

template <typename T>
void Vector<T>::close() {
  if (m_pValues)
    delete[] m_pValues; // Free allocated memory

  m_pValues = nullptr;
  m_capacity = 0;
  m_size = 0;
}

template <typename T>
size_t Vector<T>::size() const {
  return m_size;
}

template <typename T>
size_t Vector<T>::capacity() const {
  return m_capacity;
}

template <typename T>
void Vector<T>::erase(const uint64_t index) {
  if (index >= m_size)
    throw std::out_of_range("Index out of bounds");

  // Shift elements to the left to overwrite the erased element
  for (uint64_t i = index; i < m_size - 1; ++i)
    m_pValues[i] = std::move(m_pValues[i + 1]);

  pop_back(); // Reduce size
}

template <typename T>
void Vector<T>::erase(const T& remove) {
  for (uint64_t i = 0; i < m_size; ++i) {
    if (m_pValues[i] == remove) {
      erase(i); // Remove first occurrence of the value
      return;
    }
  }
}

#endif // VECTOR_TPP