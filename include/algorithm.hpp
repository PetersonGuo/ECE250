#ifndef ALG
#define ALG

#include <vector>
#include <cstdlib>

namespace algorithm {

// Returns the first index at which target can be inserted without violating
// order.
template <typename RandomAccessIterator, typename T, typename Compare>
RandomAccessIterator lower_bound(RandomAccessIterator begin,
                                 RandomAccessIterator end, const T& target,
                                 Compare comp) {
  if (begin == end)
    return begin;
  int left = 0;
  int right = end - begin - 1;
  while (left <= right) {
    int mid = left + (right - left) / 2;
    if (comp(*(begin + mid), target))
      left = mid + 1;
    else
      right = mid - 1;
  }
  return begin + left;
}

template <typename RandomAccessIterator, typename Compare>
RandomAccessIterator partition(RandomAccessIterator begin,
                               RandomAccessIterator end, Compare comp) {
  RandomAccessIterator pivot = rand() % (end - begin) + begin;
  RandomAccessIterator left = begin;
  RandomAccessIterator right = end - 1;

  std::iter_swap(pivot, right);
  pivot = right;
  --right;

  while (left <= right) {
    while (comp(*left, *pivot))
      ++left;
    while (comp(*pivot, *right))
      --right;
    if (left <= right) {
      std::iter_swap(left, right);
      ++left;
      --right;
    }
  }
  std::iter_swap(left, pivot);
  return left;
}

// Implements merge sort using the above merge.
template <typename RandomAccessIterator, typename Compare>
void sort(RandomAccessIterator begin, RandomAccessIterator end, Compare comp) {
  if (end - begin <= 1)
    return;
  RandomAccessIterator mid = algorithm::partition(begin, end, comp);
  algorithm::sort(begin, mid, comp);
  algorithm::sort(mid, end, comp);
}

} // namespace algorithm

#endif
