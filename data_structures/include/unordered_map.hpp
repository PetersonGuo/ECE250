#ifndef UNORDERED_MAP
#define UNORDERED_MAP

#include "list.hpp"
#include "pair.hpp"

template <typename T1, typename T2, typename CMP>
class Map {
private:
  // Each m_bucket holds a list of key/value pairs.
  class MapNode {
    List<Pair<T1, T2>> m_bucket;
    bool m_used;

  public:
    MapNode() : m_bucket(), m_used(false) {}
    bool used() const {
      return m_used;
    }

    bool empty() const {
      return m_bucket.empty();
    }
    friend class Map;
  };

  bool m_resizeable;
  unsigned int m_capacity; // total number of buckets
  unsigned int m_count;    // number of elements stored
  MapNode* m_data;
  CMP m_hash;

  // Compute the m_bucket index for a given key.
  unsigned int bucket_index(const T1& key) const {
    return m_hash(key) % m_capacity;
  }

  // Resize if the load factor exceeds 0.8.
  void resize() {
    if (!m_resizeable ||
        (m_count < m_capacity * 0.8 && m_count * 4 > m_capacity))
      return;
    if (m_capacity == 0)
      resize(3);
    else if (m_count * 4 <= m_capacity)
      resize(m_capacity / 2);
    else
      resize(m_capacity * 2);
  }

  void resize(unsigned int new_capacity) {
    if (new_capacity < m_count)
      return;

    MapNode* new_data = new MapNode[new_capacity];

    // Rehash all existing elements into new_data.
    for (unsigned int i = 0; i < m_capacity; ++i) {
      while (!m_data[i].m_bucket.empty()) {
        Pair<T1, T2> p = m_data[i].m_bucket.pop_front();
        unsigned int new_index = m_hash(p.getFirst()) % new_capacity;
        new_data[new_index].m_bucket.push_back(p);
      }
    }
    delete[] m_data;
    m_data = new_data;
    m_capacity = new_capacity;
  }

public:
  class Iterator {
  private:
    Map* m_map;
    unsigned int m_bucketIndex;
    typename List<Pair<T1, T2>>::Iterator m_listIt;

    void advance() {
      while (m_bucketIndex < m_map->m_capacity &&
             m_map->m_data[m_bucketIndex].m_bucket.empty())
        ++m_bucketIndex;
      if (m_bucketIndex < m_map->m_capacity)
        m_listIt = m_map->m_data[m_bucketIndex].m_bucket.begin();
    }

  public:
    Iterator(Map* map, unsigned int bucketIndex,
             typename List<Pair<T1, T2>>::Iterator listIt)
        : m_map(map), m_bucketIndex(bucketIndex), m_listIt(listIt) {}

    MapNode* operator->() const {
      return &m_map->m_data[m_bucketIndex];
    }

    Iterator& operator++() {
      ++m_listIt;
      if (m_listIt == m_map->m_data[m_bucketIndex].m_bucket.end()) {
        ++m_bucketIndex;
        advance();
      }
      return *this;
    }

    Pair<T1, T2>& operator*() const {
      return *m_listIt;
    }

    bool operator==(const Iterator& other) const {
      return m_map == other.m_map && m_bucketIndex == other.m_bucketIndex &&
             m_listIt == other.m_listIt;
    }

    bool operator!=(const Iterator& other) const {
      return !(*this == other);
    }

    friend class Map;
  };

  // Constructor.
  Map(CMP hash) : m_resizeable(true), m_capacity(3), m_count(0), m_hash(hash) {
    m_data = new MapNode[m_capacity];
  }

  // Destructor.
  ~Map() {
    delete[] m_data;
  }

  // Copy constructor (deep copy).
  Map(const Map& other)
      : m_resizeable(other.m_resizeable), m_capacity(other.m_capacity),
        m_count(other.m_count), m_hash(other.m_hash) {
    m_data = new MapNode[m_capacity];
    for (unsigned int i = 0; i < m_capacity; ++i)
      m_data[i].m_bucket = other.m_data[i].m_bucket;
  }

  // Copy assignment operator.
  Map& operator=(const Map& other) {
    if (this != &other) {
      delete[] m_data;
      m_resizeable = other.m_resizeable;
      m_capacity = other.m_capacity;
      m_count = other.m_count;
      m_hash = other.m_hash;
      m_data = new MapNode[m_capacity];
      for (unsigned int i = 0; i < m_capacity; ++i)
        m_data[i].m_bucket = other.m_data[i].m_bucket;
    }
    return *this;
  }

  // Move constructor.
  Map(Map&& other) noexcept
      : m_resizeable(other.m_resizeable), m_capacity(other.m_capacity),
        m_count(other.m_count), m_data(other.m_data),
        m_hash(std::move(other.m_hash)) {
    other.m_data = nullptr;
    other.m_capacity = 0;
    other.m_count = 0;
  }

  // Move assignment operator.
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

  T2& operator[](const T1& key) {
    unsigned int index = bucket_index(key);
    for (auto it = m_data[index].m_bucket.begin();
         it != m_data[index].m_bucket.end(); ++it) {
      if (it->getFirst() == key)
        return it->getSecond();
    }
    m_data[index].m_used = 1;
    m_data[index].m_bucket.push_back(Pair<T1, T2>(key, T2()));
    ++m_count;
    resize();
    for (auto it = m_data[index].m_bucket.begin();
         it != m_data[index].m_bucket.end(); ++it) {
      if (it->getFirst() == key)
        return it->getSecond();
    }
    static T2 dummy;
    return dummy;
  }

  Iterator operator[](const unsigned int index) {
    if (index >= m_capacity)
      throw std::out_of_range("Index out of bounds");
    return Iterator(this, index, m_data[index].m_bucket.begin());
  }

  const T2& at(const T1& key) const {
    unsigned int index = bucket_index(key);
    for (auto it = m_data[index].m_bucket.begin();
         it != m_data[index].m_bucket.end(); ++it) {
      if (it->getFirst() == key)
        return it->getSecond();
    }
    static T2 dummy;
    return dummy;
  }

  // Check whether key exists in the map.
  bool contains(const T1& key) const {
    unsigned int index = bucket_index(key);
    for (auto it = m_data[index].m_bucket.begin();
         it != m_data[index].m_bucket.end(); ++it) {
      if (it->getFirst() == key)
        return true;
    }
    return false;
  }

  // Erase an element by key.
  void erase(const T1& key) {
    unsigned int index = bucket_index(key);
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

  void erase(Iterator& it) {
    m_data[it.m_bucketIndex].m_bucket.erase(it.m_listIt);
    it = end();
    --m_count;
    resize();
  }

  void clear() {
    delete[] m_data;
    m_capacity = 3;
    m_data = new MapNode[m_capacity];
    m_count = 0;
  }

  Iterator begin() {
    for (unsigned int i = 0; i < m_capacity; ++i) {
      if (!m_data[i].m_bucket.empty())
        return Iterator(this, i, m_data[i].m_bucket.begin());
    }
    return end();
  }

  Iterator end() {
    return Iterator(this, m_capacity,
                    typename List<Pair<T1, T2>>::Iterator(nullptr));
  }

  Iterator begin() const {
    for (unsigned int i = 0; i < m_capacity; ++i) {
      if (!m_data[i].m_bucket.empty())
        return Iterator(const_cast<Map*>(this), i, m_data[i].m_bucket.begin());
    }
    return end();
  }

  Iterator end() const {
    return Iterator(const_cast<Map*>(this), m_capacity,
                    typename List<Pair<T1, T2>>::Iterator(nullptr));
  }

  Iterator find(const T1& key) {
    unsigned int index = bucket_index(key);
    for (auto it = m_data[index].m_bucket.begin();
         it != m_data[index].m_bucket.end(); ++it) {
      if (it->getFirst() == key)
        return Iterator(this, index, it);
    }
    return end();
  }

  Iterator find(const T1& key) const {
    unsigned int index = bucket_index(key);
    for (auto it = m_data[index].m_bucket.begin();
         it != m_data[index].m_bucket.end(); ++it) {
      if (it->getFirst() == key)
        return Iterator(const_cast<Map*>(this), index, it);
    }
    return end();
  }

  unsigned int capacity() const {
    return m_capacity;
  }

  void set_capacity(unsigned int capacity) {
    if (capacity < m_count)
      return;
    resize(capacity);
    m_resizeable = false;
  }

  unsigned int size() const {
    return m_count;
  }

  bool was_used(unsigned int index) const {
    return m_data[index].m_used;
  }
};

#endif
