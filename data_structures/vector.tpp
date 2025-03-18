/*
  Implementation of the Vector class template.
  This file contains the definitions of the Vector class template methods,
  which provide dynamic array functionality. It includes methods for resizing,
  inserting, removing, and accessing elements, along with memory management.
*/

#include <cassert>
#include <stdexcept>

/*
  Reallocates memory for the vector based on its current size and capacity.
  Doubles the capacity if the vector is full and halves it if the size is less
  than a quarter of the capacity.
*/
template <typename T> void Vector<T>::reallocate() {
  if (m_size == m_capacity) {
    m_capacity <<= 1;
    resize(m_capacity);
  } else if (m_size * 4 <= m_capacity) {
    m_capacity >>= 1;
    resize(m_capacity);
  }
}

/*
  Constructs a Vector with the specified initial size.
  Initializes the capacity to at least 2 and allocates memory accordingly.
  @param size Initial size of the vector.
*/
template <typename T>
Vector<T>::Vector(std::size_t size) : m_size(size), m_pValues(nullptr) {
  m_capacity = size < 2 ? 2 : size;
  resize(m_capacity);
}

/*
  Destructor for the Vector class.
  Frees the allocated memory for the vector.
*/
template <typename T> Vector<T>::~Vector() { delete[] m_pValues; }

/*
  Accesses the element at the specified index.
  Throws an exception if the index is out of bounds.
  @param index Index of the element to access.
  @return Reference to the element at the specified index.
*/
template <typename T> T &Vector<T>::operator[](uint64_t index) {
  if (index >= m_size)
    throw std::out_of_range("Index out of bounds");
  return m_pValues[index];
}

/*
  Const version of the operator[] for accessing elements.
  @param loc Index of the element to access.
  @return Const reference to the element at the specified index.
*/
template <typename T> const T &Vector<T>::operator[](uint64_t loc) const {
  if (loc >= m_size)
    throw std::out_of_range("Index out of bounds");
  return m_pValues[loc];
}

/*
  Returns a pointer to the beginning of the vector.
  @return Pointer to the first element in the vector.
*/
template <typename T> T *Vector<T>::begin() { return m_pValues; }

/*
  Returns a pointer to the end of the vector (one past the last element).
  @return Pointer to one past the last element in the vector.
*/
template <typename T> T *Vector<T>::end() { return m_pValues + m_size; }

/*
  Returns a pointer to the first element in the vector.
  @return Pointer to the first element.
*/
template <typename T> T *Vector<T>::front() { return *m_pValues; }

/*
  Returns a pointer to the last element in the vector.
  @return Pointer to the last element.
*/
template <typename T> T *Vector<T>::back() { return m_pValues[m_size - 1]; }

/*
  Resizes the vector to the specified size by allocating new memory and copying
  the existing elements to the new memory.
  @param size New capacity of the vector.
*/
template <typename T> void Vector<T>::resize(std::size_t size) {
  T *temp = new T[size];
  assert(temp && "Could not allocate space for new array");
  if (m_pValues) {
    memcpy(temp, m_pValues, sizeof(T) * m_size);
    delete[] m_pValues;
  }
  m_pValues = temp;
}

/*
  Inserts a value at the specified index in the vector.
  Shifts all elements after the index to the right and adjusts the size.
  @param index Position to insert the value.
  @param value Value to insert into the vector.
*/
template <typename T>
void Vector<T>::insert(const uint64_t index, const T &value) {
  reallocate();
  if (index == m_size) {
    push_back(value);
  } else {
    memcpy(m_pValues + index + 1, m_pValues + index,
           sizeof(T) * (m_size - index));
    ++m_size;
    m_pValues[index] = value;
  }
}

/*
  Appends a value to the end of the vector.
  Resizes the vector if necessary to accommodate the new element.
  @param value Value to append to the vector.
*/
template <typename T> void Vector<T>::push_back(const T &value) {
  reallocate();
  m_pValues[m_size++] = value;
}

/*
  Removes the last element in the vector.
  Reduces the size and reallocates memory if necessary.
*/
template <typename T> void Vector<T>::pop_back() {
  --m_size;
  m_pValues[m_size] = 0;
  reallocate();
}

/*
  Clears the contents of the vector.
  Resets the size and capacity to the default minimum values.
*/
template <typename T> void Vector<T>::clear() {
  m_capacity = 2;
  m_size = 0;
  resize(m_capacity);
}

/*
  Returns the current size of the vector (number of elements).
  @return The size of the vector.
*/
template <typename T> std::size_t Vector<T>::size() { return m_size; }

/*
  Returns the current capacity of the vector (allocated memory).
  @return The capacity of the vector.
*/
template <typename T> std::size_t Vector<T>::capacity() { return m_capacity; }
