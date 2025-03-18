/*
  Implementation of the Pair class template.
  This file contains the definitions of the Pair class template methods,
  which encapsulate a pair of values of potentially different types.
  It includes functionality for construction, assignment, comparison,
  and access to the pair's elements.
*/

#include <iostream>

/*
  Default constructor for the Pair class.
  Initializes the first and second elements to their default values.
*/
template <typename T1, typename T2>
Pair<T1, T2>::Pair() : first(T1()), second(T2()) {}

/*
  Parameterized constructor for the Pair class.
  Initializes the pair with the specified values.
  @param first Value for the first element.
  @param second Value for the second element.
*/
template <typename T1, typename T2>
Pair<T1, T2>::Pair(const T1 &first, const T2 &second)
    : first(first), second(second) {}

/*
  Copy constructor for the Pair class.
  Creates a new Pair object by copying the values of another Pair.
  @param other The Pair object to copy from.
*/
template <typename T1, typename T2>
Pair<T1, T2>::Pair(const Pair &other)
    : first(other.first), second(other.second) {}

/*
  Move constructor for the Pair class.
  Transfers ownership of the values from another Pair.
  @param other The Pair object to move from.
*/
template <typename T1, typename T2>
Pair<T1, T2>::Pair(Pair &&other)
    : first(std::move(other.first)), second(std::move(other.second)) {}

/*
  Copy assignment operator for the Pair class.
  Copies the values from another Pair into this Pair.
  @param other The Pair object to copy from.
  @return Reference to the updated Pair object.
*/
template <typename T1, typename T2>
Pair<T1, T2> &Pair<T1, T2>::operator=(const Pair &other) {
  if (this != &other) {
    first = other.first;
    second = other.second;
  }
  return *this;
}

/*
  Move assignment operator for the Pair class.
  Moves the values from another Pair into this Pair.
  @param other The Pair object to move from.
  @return Reference to the updated Pair object.
*/
template <typename T1, typename T2>
Pair<T1, T2> &Pair<T1, T2>::operator=(Pair &&other) {
  if (this != &other) {
    first = std::move(other.first);
    second = std::move(other.second);
  }
  return *this;
}

/*
  Equality operator for the Pair class.
  Compares two Pair objects for equality based on their elements.
  @param other The Pair object to compare with.
  @return True if the pairs are equal, false otherwise.
*/
template <typename T1, typename T2>
bool Pair<T1, T2>::operator==(const Pair &other) const {
  return first == other.first && second == other.second;
}

/*
  Inequality operator for the Pair class.
  Compares two Pair objects for inequality based on their elements.
  @param other The Pair object to compare with.
  @return True if the pairs are not equal, false otherwise.
*/
template <typename T1, typename T2>
bool Pair<T1, T2>::operator!=(const Pair &other) const {
  return !(*this == other);
}

/*
  Accessor for the first element of the Pair.
  @return Reference to the first element.
*/
template <typename T1, typename T2> T1 &Pair<T1, T2>::getFirst() {
  return first;
}

/*
  Accessor for the second element of the Pair.
  @return Reference to the second element.
*/
template <typename T1, typename T2> T2 &Pair<T1, T2>::getSecond() {
  return second;
}
