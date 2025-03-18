#ifndef UNORDERED_MAP
#define UNORDERED_MAP

#include "list.hpp"
#include "pair.hpp"

template <typename T1, typename T2, typename CMP>
/*
 * Map class represents an unordered map data structure that stores key-value
 * pairs. It provides functionality to insert, erase, and access elements by
 * key, as well as iterate over the map.
 */
class Map {
private:
  /*
   * MapNode class represents a bucket in the map that stores key-value pairs.
   */
  class MapNode {
    List<Pair<T1, T2>> m_bucket;
    bool m_used;

  public:
    /*
     * Default constructor.
     */
    MapNode() : m_bucket(), m_used(false) {}

    /*
     * Check if the bucket has been used.
     * @return True if the bucket has been used, false otherwise.
     */
    bool used() const {
      return m_used;
    }

    /*
     * Check if the bucket is empty.
     * @return True if the bucket is empty, false otherwise.
     */
    bool empty() const {
      return m_bucket.empty();
    }
    friend class Map;
  };

  bool m_resizeable;             // flag to enable resizing
  unsigned long long m_capacity; // total number of buckets
  unsigned long long m_count;    // number of elements stored
  MapNode* m_data;               // array of MapNode objects
  CMP m_hash;                    // hash function

  /*
   * Compute the bucket index for a given key.
   * @param key The key to hash.
   * @return The index of the bucket where the key should be stored.
   */
  unsigned long long bucket_index(const T1& key) const {
    return m_hash(key) % m_capacity;
  }

  /*
   * Resize the map if necessary.
   * The map is resized if the number of elements exceeds 80% of the capacity
   * or if the number of elements is less than 40% of the capacity.
   */
  void resize() {
    if (!m_resizeable ||
        (m_count < m_capacity * 0.8 && m_count * 4 > m_capacity))
      return;
    if (m_capacity < 3)
      resize(3);
    else if (m_capacity == 3)
      return;
    else if (m_count * 4 <= m_capacity)
      resize(m_capacity / 2);
    else if (m_count >= m_capacity * 0.8)
      resize(m_capacity * 2);
  }

  /*
   * Resize the map to a new capacity.
   * @param new_capacity The new capacity of the map.
   */
  void resize(unsigned long long new_capacity) {
    if (new_capacity < m_count)
      return;

    MapNode* new_data = new MapNode[new_capacity];

    for (unsigned long long i = 0; i < new_capacity; ++i)
      new_data[i].m_bucket = List<Pair<T1, T2>>();

    // Rehash all existing elements into new_data.
    for (unsigned long long i = 0; i < m_capacity; ++i) {
      while (!m_data[i].m_bucket.empty()) {
        Pair<T1, T2> p = m_data[i].m_bucket.pop_front();
        unsigned long long new_index = m_hash(p.getFirst()) % new_capacity;
        new_data[new_index].m_bucket.push_back(p);
      }
    }
    if (m_data)
      delete[] m_data;
    m_data = new_data;
    m_capacity = new_capacity;
  }

  /*
   * Access an element by key.
   * @param key The key to access.
   * @return A reference to the value associated with the key.
   */
  T2& at_internal(const T1& key) {
    unsigned long long index = bucket_index(key);
    for (auto it = m_data[index].m_bucket.begin();
         it != m_data[index].m_bucket.end(); ++it) {
      if (it->getFirst() == key)
        return it->getSecond();
    }
    throw std::out_of_range("Key not found");
  }

public:
  /*
   * Iterator class provides functionality to iterate over the map.
   */
  class Iterator {
  private:
    Map* m_map;                                     // reference to the map
    unsigned long long m_bucketIndex;               // current bucket index
    typename List<Pair<T1, T2>>::Iterator m_listIt; // current list iterator

    /*
     * Advance the iterator to the next non-empty bucket.
     */
    void advance() {
      while (m_bucketIndex < m_map->m_capacity &&
             m_map->m_data[m_bucketIndex].m_bucket.empty())
        ++m_bucketIndex;
      if (m_bucketIndex < m_map->m_capacity)
        m_listIt = m_map->m_data[m_bucketIndex].m_bucket.begin();
    }

  public:
    /*
     * Constructor.
     * @param map The map to iterate over.
     * @param bucketIndex The current bucket index.
     * @param listIt The current list iterator.
     */
    Iterator(Map* map, unsigned long long bucketIndex,
             typename List<Pair<T1, T2>>::Iterator listIt)
        : m_map(map), m_bucketIndex(bucketIndex), m_listIt(listIt) {}

    /*
     * Dereference operator.
     */
    MapNode* operator->() const {
      return &m_map->m_data[m_bucketIndex];
    }

    /*
     * Prefix increment operator.
     */
    Iterator& operator++() {
      ++m_listIt;
      if (m_listIt == m_map->m_data[m_bucketIndex].m_bucket.end()) {
        ++m_bucketIndex;
        advance();
      }
      return *this;
    }

    /*
     * Dereference operator.
     */
    Pair<T1, T2>& operator*() const {
      return *m_listIt;
    }

    /*
     * Equality operator.
     */
    bool operator==(const Iterator& other) const {
      return m_map == other.m_map && m_bucketIndex == other.m_bucketIndex &&
             m_listIt == other.m_listIt;
    }

    /*
     * Inequality operator.
     */
    bool operator!=(const Iterator& other) const {
      return !(*this == other);
    }

    friend class Map;
  };

  /*
   * Default constructor.
   */
  Map(CMP hash)
      : m_resizeable(true), m_capacity(0), m_count(0), m_data(nullptr),
        m_hash(hash) {
    resize(3);
  }

  /*
   * Destructor.
   */
  ~Map() {
    delete[] m_data;
  }

  /*
   * Copy constructor.
   */
  Map(const Map& other)
      : m_resizeable(other.m_resizeable), m_capacity(other.m_capacity),
        m_count(other.m_count), m_hash(other.m_hash) {
    m_data = new MapNode[m_capacity];
    for (unsigned long long i = 0; i < m_capacity; ++i)
      m_data[i].m_bucket = other.m_data[i].m_bucket;
  }

  /*
   * Copy assignment operator.
   */
  Map& operator=(const Map& other) {
    if (this != &other) {
      delete[] m_data;
      m_resizeable = other.m_resizeable;
      m_capacity = other.m_capacity;
      m_count = other.m_count;
      m_hash = other.m_hash;
      m_data = new MapNode[m_capacity];
      for (unsigned long long i = 0; i < m_capacity; ++i)
        m_data[i].m_bucket = other.m_data[i].m_bucket;
    }
    return *this;
  }

  /*
   * Move constructor.
   */
  Map(Map&& other) noexcept
      : m_resizeable(other.m_resizeable), m_capacity(other.m_capacity),
        m_count(other.m_count), m_data(other.m_data),
        m_hash(std::move(other.m_hash)) {
    other.m_data = nullptr;
    other.m_capacity = 0;
    other.m_count = 0;
  }

  /*
   * Move assignment operator.
   */
  Map& operator=(Map&& other) noexcept {
    if (this != &other) {
      delete[] m_data;
      m_resizeable = other.m_resizeable;
      m_capacity = other.m_capacity;
      m_count = other.m_count;
      m_data = other.m_data;
      m_hash = std::move(other.m_hash);
      other.m_data = nullptr;
      other.m_capacity = 0;
      other.m_count = 0;
    }
    return *this;
  }

  /*
   * Access an element by key.
   * @param key The key to access.
   * @return A reference to the value associated with the key.
   */
  T2& operator[](const T1& key) {
    if (contains(key))
      return at_internal(key);
    ++m_count;
    resize();
    unsigned long long index = bucket_index(key);
    m_data[index].m_bucket.push_back(Pair<T1, T2>(key, T2()));
    m_data[index].m_used = 1;
    return at_internal(key);
  }

  /*
   * Access an element by key.
   * @param key The key to access.
   * @return A reference to the value associated with the key.
   */
  Iterator operator[](const unsigned long long index) {
    if (index >= m_capacity)
      throw std::out_of_range("Index out of bounds");
    return Iterator(this, index, m_data[index].m_bucket.begin());
  }

  /*
   * Access an element by key.
   * @param key The key to access.
   * @return A reference to the value associated with the key.
   */
  const T2& at(const T1& key) const {
    unsigned long long index = bucket_index(key);
    for (auto it = m_data[index].m_bucket.begin();
         it != m_data[index].m_bucket.end(); ++it) {
      if (it->getFirst() == key)
        return it->getSecond();
    }
    throw std::out_of_range("Key not found");
  }

  /*
   * Check if the map contains a key.
   * @param key The key to search for.
   * @return True if the key is found, false otherwise.
   */
  bool contains(const T1& key) const {
    unsigned long long index = bucket_index(key);
    for (auto it = m_data[index].m_bucket.begin();
         it != m_data[index].m_bucket.end(); ++it) {
      if (it->getFirst() == key)
        return true;
    }
    return false;
  }

  /*
   * Erase an element by key.
   * @param key The key to erase.
   */
  void erase(const T1& key) {
    unsigned long long index = bucket_index(key);
    for (auto it = m_data[index].m_bucket.begin();
         it != m_data[index].m_bucket.end(); ++it) {
      if (it->getFirst() == key) {
        m_data[index].m_bucket.erase(it);
        --m_count;
        resize();
        return;
      }
    }
  }

  /*
   * Erase an element by iterator.
   * @param it The iterator to erase.
   */
  void erase(Iterator& it) {
    m_data[it.m_bucketIndex].m_bucket.erase(it.m_listIt);
    it = end();
    --m_count;
    resize();
  }

  /*
   * Clear all elements from the map.
   */
  void clear() {
    delete[] m_data;
    m_capacity = 3;
    m_data = new MapNode[m_capacity];
    m_count = 0;
  }

  /*
   * Get an iterator to the beginning of the map.
   * @return An iterator to the beginning of the map.
   */
  Iterator begin() {
    for (unsigned long long i = 0; i < m_capacity; ++i) {
      if (!m_data[i].m_bucket.empty())
        return Iterator(this, i, m_data[i].m_bucket.begin());
    }
    return end();
  }

  /*
   * Get an iterator to the beginning of the map.
   * @return An iterator to the beginning of the map.
   */
  Iterator begin() const {
    for (unsigned long long i = 0; i < m_capacity; ++i) {
      if (!m_data[i].m_bucket.empty())
        return Iterator(const_cast<Map*>(this), i, m_data[i].m_bucket.begin());
    }
    return end();
  }

  /*
   * Get an iterator to the end of the map.
   * @return An iterator to the end of the map.
   */
  Iterator end() {
    return Iterator(this, m_capacity,
                    typename List<Pair<T1, T2>>::Iterator(nullptr));
  }

  /*
   * Get an iterator to the end of the map.
   * @return An iterator to the end of the map.
   */
  Iterator end() const {
    return Iterator(const_cast<Map*>(this), m_capacity,
                    typename List<Pair<T1, T2>>::Iterator(nullptr));
  }

  /*
   * Find an element by key.
   * @param key The key to find.
   * @return An iterator to the element if found, end() otherwise.
   */
  Iterator find(const T1& key) {
    unsigned long long index = bucket_index(key);
    for (auto it = m_data[index].m_bucket.begin();
         it != m_data[index].m_bucket.end(); ++it) {
      if (it->getFirst() == key)
        return Iterator(this, index, it);
    }
    return end();
  }

  /*
   * Find an element by key.
   * @param key The key to find.
   * @return An iterator to the element if found, end() otherwise.
   */
  Iterator find(const T1& key) const {
    unsigned long long index = bucket_index(key);
    for (auto it = m_data[index].m_bucket.begin();
         it != m_data[index].m_bucket.end(); ++it) {
      if (it->getFirst() == key)
        return Iterator(const_cast<Map*>(this), index, it);
    }
    return end();
  }

  /*
   * Get the number of elements stored in the map.
   * @return The number of elements stored in the map.
   */
  unsigned long long capacity() const {
    return m_capacity;
  }

  /*
   * Set the number of elements stored in the map.
   * @param capacity The number of elements stored in the map.
   */
  void set_capacity(unsigned long long capacity) {
    if (capacity < m_count)
      return;
    resize(capacity);
    m_resizeable = false;
  }

  /*
   * Get the number of elements stored in the map.
   * @return The number of elements stored in the map.
   */
  unsigned long long size() const {
    return m_count;
  }

  /*
   * Check if a map element is used.
   * @return True if the element was used, false otherwise.
   */
  bool was_used(unsigned long long index) const {
    return m_data[index].m_used;
  }
};

#endif
