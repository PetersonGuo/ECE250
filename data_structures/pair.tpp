/*
  Implementation of the Pair class template.
  This file contains the definitions of the Pair class template methods,
  which encapsulate a pair of values of potentially different types.
  It includes functionality for construction, assignment, comparison,
  and access to the pair's elements.
*/

#include <utility>

template <typename T1, typename T2>
Pair<T1, T2>::Pair() : first(T1()), second(T2())
{}

template <typename T1, typename T2>
Pair<T1, T2>::Pair(const T1& first, const T2& second)
    : first(first), second(second)
{}

template <typename T1, typename T2>
Pair<T1, T2>::Pair(const Pair& other) : first(other.first), second(other.second)
{}

template <typename T1, typename T2>
Pair<T1, T2>::Pair(Pair&& other)
    : first(std::move(other.first)), second(std::move(other.second))
{}

template <typename T1, typename T2>
Pair<T1, T2>& Pair<T1, T2>::operator=(const Pair& other) noexcept
{
  if (this != &other) {
    first = other.first;
    second = other.second;
  }
  return *this;
}

<<<<<<< HEAD
/*
  Move assignment operator for the Pair class.
  Moves the values from another Pair into this Pair.
  @param other The Pair object to move from.
  @return Reference to the updated Pair object.
*/
template <typename T1, typename T2>
Pair<T1, T2> &Pair<T1, T2>::operator=(Pair &&other) {
=======
template <typename T1, typename T2>
Pair<T1, T2>& Pair<T1, T2>::operator=(Pair&& other) noexcept
{
>>>>>>> 28192f3 (P2)
  if (this != &other) {
    first = std::move(other.first);
    second = std::move(other.second);
  }
  return *this;
}

template <typename T1, typename T2>
bool Pair<T1, T2>::operator==(const Pair& other) const
{
  return first == other.first && second == other.second;
}

template <typename T1, typename T2>
bool Pair<T1, T2>::operator!=(const Pair& other) const
{
  return !(*this == other);
}

template <typename T1, typename T2>
T1& Pair<T1, T2>::getFirst()
{
  return first;
}

template <typename T1, typename T2>
const T1& Pair<T1, T2>::getFirst() const
{
  return first;
}

template <typename T1, typename T2>
T2& Pair<T1, T2>::getSecond()
{
  return second;
}

template <typename T1, typename T2>
const T2& Pair<T1, T2>::getSecond() const
{
  return second;
}
