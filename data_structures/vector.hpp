/*
  Vector class header file.
  This file defines a template-based dynamic array class called Vector.
  The class provides methods for accessing, modifying, and managing elements,
  as well as handling memory allocation dynamically.
*/

#ifndef VECTOR
#define VECTOR

#include <cstdint>
#include <cstring>

/*
  Template-based dynamic array class.
  Provides functionality similar to std::vector, including dynamic resizing,
  element access, and memory management.
  @tparam T Type of elements stored in the vector.
*/
template <typename T> class Vector {
private:
  std::size_t m_capacity; ///< Maximum capacity of the dynamic array.
  std::size_t m_size;     ///< Current number of elements in the array.
  T *m_pValues;           ///< Pointer to the values stored in the array.

  /*
    Handles memory reallocation when the array size exceeds capacity
    or becomes significantly smaller than capacity.
  */
  void reallocate();

public:
  /*
    Constructor for the Vector class.
    Initializes the vector with an optional initial size.
    @param size Initial size of the vector (default is 0).
  */
  Vector(std::size_t size = 0);

  /*
    Destructor for the Vector class.
    Releases the memory allocated for the array.
  */
  ~Vector();

  /*
    Overloaded subscript operator for accessing elements by index.
    Throws an exception if the index is out of bounds.
    @param index Index of the element to access.
    @return Reference to the element at the specified index.
  */
  T &operator[](uint64_t index);

  /*
    Const version of the subscript operator for accessing elements by index.
    @param index Index of the element to access.
    @return Const reference to the element at the specified index.
  */
  const T &operator[](uint64_t index) const;

  /*
    Returns a pointer to the beginning of the vector.
    @return Pointer to the first element.
  */
  T *begin();

  /*
    Returns a pointer to the end of the vector (one past the last element).
    @return Pointer to one past the last element.
  */
  T *end();

  /*
    Returns a pointer to the first element of the vector.
    @return Pointer to the first element.
  */
  T *front();

  /*
    Returns a pointer to the last element of the vector.
    @return Pointer to the last element.
  */
  T *back();

  /*
    Resizes the vector to a new capacity.
    Allocates new memory and copies existing elements to the new memory.
    @param size New capacity of the vector.
  */
  void resize(std::size_t size);

  /*
    Inserts a value at a specific index in the vector.
    Shifts all subsequent elements to the right to make space.
    @param index Position where the value will be inserted.
    @param value Value to insert into the vector.
  */
  void insert(const uint64_t index, const T &value);

  /*
    Appends a value to the end of the vector.
    Resizes the vector if needed to accommodate the new value.
    @param value Value to append to the vector.
  */
  void push_back(const T &value);

  /*
    Removes the last element from the vector.
    Reduces the size and reallocates memory if needed.
  */
  void pop_back();

  /*
    Clears the contents of the vector.
    Resets the size and capacity to the default minimum values.
  */
  void clear();

  /*
    Returns the current number of elements in the vector.
    @return The size of the vector.
  */
  std::size_t size();

  /*
    Returns the current capacity of the vector (allocated memory).
    @return The capacity of the vector.
  */
  std::size_t capacity();
};

#include "vector.tpp"
#endif
