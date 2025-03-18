#ifndef ALG
#define ALG

#include "vector.hpp"

/*
  The `less` comparator class.
  Used to compare two values and return -1, 1, or 0 based on their relation.
  This follows the behavior of a three-way comparison:
  - Returns -1 if a < b
  - Returns 1 if a > b
  - Returns 0 if a == b
*/
template <typename T>
class less {
  int operator()(const T& a, const T& b) const {
    if (a < b)
      return -1;
    if (b < a)
      return 1;
    return 0;
  }
};

/*
  The `greater` comparator class.
  Similar to `less`, but operates in reverse order:
  - Returns -1 if a > b
  - Returns 1 if a < b
  - Returns 0 if a == b
*/
template <typename T>
class greater {
  int operator()(const T& a, const T& b) const {
    if (a > b)
      return -1;
    if (b > a)
      return 1;
    return 0;
  }
};

/*
  Merges two sorted halves of an array into a single sorted sequence.
  This function is used as part of the merge sort algorithm.

  @param begin Pointer to the beginning of the array.
  @param mid Pointer to the middle of the array.
  @param end Pointer to the end of the array.
  @param cmp Comparator function that determines sorting order.
*/
template <typename T, typename CMP>
void merge(T* begin, T* mid, T* end, CMP cmp) {
  size_t n = end - begin;
  Vector<T> temp(n);

  T* l = begin; // Left half iterator
  T* r = mid;   // Right half iterator
  size_t idx = 0;

  // Merge elements from both halves into temp vector
  while (l < mid && r < end) {
    if (cmp(*l, *r))
      temp[idx++] = *l++;
    else
      temp[idx++] = *r++;
  }

  // Copy any remaining elements from left half
  while (l < mid)
    temp[idx++] = *l++;

  // Copy any remaining elements from right half
  while (r < end)
    temp[idx++] = *r++;

  // Copy the sorted elements back to the original array
  unsigned int i = 0;
  for (T* it = temp.begin(); it != temp.end(); ++it, ++i) {
    begin[i] = *it;
  }
}

/*
  Implements merge sort using recursion.

  @param begin Pointer to the beginning of the array.
  @param end Pointer to the end of the array.
  @param cmp Comparator function that determines sorting order.
*/
template <typename T, typename CMP>
void sort(T* begin, T* end, CMP cmp) {
  if (end - begin <= 1)
    return;

  T* mid = begin + (end - begin) / 2;

  // Recursively sort both halves
  sort(begin, mid, cmp);
  sort(mid, end, cmp);

  // Merge sorted halves
  merge(begin, mid, end, cmp);
}

/*
  Finds the first position in a sorted range where a given value could be
  inserted while maintaining order.

  @param first Pointer to the first element in the range.
  @param last Pointer to the last element in the range.
  @param value The value to compare.
  @param comp Comparator function that determines sorting order.
  @return Pointer to the first position where `value` could be inserted.
*/
template <typename T, typename U, typename Compare>
T* lower_bound(T* first, T* last, const U& value, Compare comp) {
  int count = last - first;

  while (count > 0) {
    int step = count / 2;
    T* it = first + step;

    // If `comp(*it, value)` is true, move `first` forward
    if (comp(*it, value)) {
      first = it + 1;
      count = count - step - 1;
    } else {
      count = step;
    }
  }
  return first;
}

#endif
