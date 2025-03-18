/*
  Vector class header file.
  This file defines a template-based dynamic array class called Vector.
  The class provides methods for accessing, modifying, and managing elements,
  as well as handling dynamic memory allocation for efficient resizing.
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
template <typename T>
class Vector
{
private:
  size_t m_capacity; // Maximum capacity of the dynamic array.
  size_t m_size;     // Current number of elements in the array.
  T* m_pValues;      // Pointer to the values stored in the array.

  /*
    Handles memory reallocation when the array size exceeds capacity
    or becomes significantly smaller than capacity.
  */
  void reallocate();

public:
  /**
   * @brief Constructs a Vector with an optional initial size.
   * @param size Initial size of the vector (default is 0).
   */
  Vector(const size_t = 0);

  /**
   * @brief Copy constructor for Vector.
   * @param other The Vector object to copy from.
   */
  Vector(const Vector&);

  /**
   * @brief Copy assignment operator.
   * @param other The Vector object to assign from.
   * @return A reference to this Vector object.
   */
  Vector& operator=(const Vector& other);

  /**
   * @brief Move constructor for Vector.
   * Transfers ownership of resources from another Vector object.
   * @param other The Vector object to move from.
   */
  Vector(Vector&&) noexcept;

  /**
   * @brief Move assignment operator.
   * Transfers ownership of resources from another Vector object.
   * @param other The Vector object to move from.
   * @return A reference to this Vector object.
   */
  Vector& operator=(Vector&&) noexcept;

  /**
   * @brief Destructor for the Vector class.
   * Frees the allocated memory.
   */
  ~Vector();

  /**
   * @brief Overloaded subscript operator for accessing elements by index.
   * @param index Index of the element to access.
   * @return Reference to the element at the specified index.
   * @throws std::out_of_range if index is out of bounds.
   */
  T& operator[](const uint64_t);

  /**
   * @brief Const version of the subscript operator for accessing elements.
   * @param index Index of the element to access.
   * @return Const reference to the element at the specified index.
   * @throws std::out_of_range if index is out of bounds.
   */
  const T& operator[](const uint64_t) const;

  /**
   * @brief Returns a pointer to the beginning of the vector.
   * @return Pointer to the first element.
   */
  T* begin();
  const T* begin() const;

  /**
   * @brief Returns a pointer to the end of the vector (one past the last
   * element).
   * @return Pointer to one past the last element.
   */
  T* end();
  const T* end() const;

  /**
   * @brief Returns a reference to the first element of the vector.
   * @return Reference to the first element.
   */
  T& front();

  /**
   * @brief Returns a reference to the last element of the vector.
   * @return Reference to the last element.
   */
  T& back();

  /**
   * @brief Resizes the vector to a new capacity.
   * Allocates new memory and copies existing elements to the new memory.
   * @param size New capacity of the vector.
   */
  void resize(const size_t);

  /**
   * @brief Inserts a value at a specific index in the vector.
   * Shifts all subsequent elements to the right to make space.
   * @param index Position where the value will be inserted.
   * @param value Value to insert into the vector.
   * @throws std::out_of_range if index is out of bounds.
   */
  void insert(const uint64_t, const T&);

  /**
   * @brief Constructs and inserts an element at a specific index.
   * @param index Position where the value will be inserted.
   * @param args Arguments to construct the new element.
   * @throws std::out_of_range if index is out of bounds.
   */
  template <class... Args>
  void emplace(const uint64_t, Args&&...);

  /**
   * @brief Appends a value to the end of the vector.
   * Resizes the vector if needed to accommodate the new value.
   * @param value Value to append to the vector.
   */
  void push_back(const T&);

  /**
   * @brief Constructs and appends an element to the end of the vector.
   * @param args Arguments to construct the new element.
   */
  template <class... Args>
  void emplace_back(Args&&...);

  /**
   * @brief Removes the last element from the vector.
   * Reduces the size and reallocates memory if needed.
   * @throws std::out_of_range if the vector is empty.
   */
  void pop_back();

  /**
   * @brief Clears the contents of the vector.
   * Resets the size and capacity to the default minimum values.
   */
  void clear();

  /**
   * @brief Releases the memory allocated by the vector.
   */
  void close();

  /**
   * @brief Returns the current number of elements in the vector.
   * @return The size of the vector.
   */
  const size_t size() const;

  /**
   * @brief Returns the current capacity of the vector (allocated memory).
   * @return The capacity of the vector.
   */
  const size_t capacity() const;

  /**
   * @brief Removes an element at a specific index.
   * Shifts subsequent elements to the left to fill the gap.
   * @param index The index of the element to remove.
   * @throws std::out_of_range if index is out of bounds.
   */
  void erase(const uint64_t);

  /**
   * @brief Removes the first occurrence of a specific value from the vector.
   * @param remove The value to be removed.
   */
  void erase(const T&);
};

#include "vector.tpp"
#endif
