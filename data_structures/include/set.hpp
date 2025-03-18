#ifndef SET
#define SET
#include "algorithm.hpp"

/*
  Set class.
  A template-based balanced binary search tree (BST) that stores unique values.
  Provides standard operations such as insertion, lookup (find, lower_bound),
  and iteration using a custom comparator.

  @tparam T The type of elements stored in the set.
  @tparam CMP A comparator class used for custom ordering of elements (default
  is less<T>).
*/
template <typename T, typename CMP = less<T>>
class Set {
private:
  /*
    TreeNode class.
    Represents a single node in the binary search tree.
    Each node stores a value, pointers to left and right child nodes, and its
    rank.
  */
  class TreeNode {
  private:
    T m_data;          // Data stored in the node
    TreeNode* m_left;  // Pointer to left child node
    TreeNode* m_right; // Pointer to right child node
    int m_rank;        // Rank of the node for balancing purposes

  public:
    /**
     * @brief Constructor that initializes a node with a given value.
     * @param data The value to store in the node.
     */
    TreeNode(T data) : TreeNode(data, nullptr, nullptr) {}

    /**
     * @brief Constructor that initializes a node with value and children.
     * @param data The value to store in the node.
     * @param left Pointer to the left child node.
     * @param right Pointer to the right child node.
     */
    TreeNode(T data, TreeNode* left, TreeNode* right)
        : m_data(data), m_left(left), m_right(right), m_rank(0) {}

    friend class Set;
    friend class SetIterator;
  };

  CMP m_cmp;        // Comparator function for ordering elements
  void* m_ctx;      // Optional context pointer for custom comparison logic
  TreeNode* m_root; // Root of the binary search tree

  /**
   * @brief Rebalances the tree (not yet implemented).
   */
  void rebalance() {}

  /**
   * @brief Recursively deletes all nodes in the tree.
   * @param root The root node to start deletion from.
   */
  void clear_internal(TreeNode* root) {
    if (!root)
      return;
    clear_internal(root->m_left);
    clear_internal(root->m_right);
    delete root;
  }

public:
  /**
   * @brief Constructs an empty Set.
   * @param cmp Comparator function for custom ordering (default is less<T>).
   * @param ctx Optional context pointer for comparison.
   */
  Set(CMP cmp = CMP(), void* ctx = nullptr)
      : m_cmp(cmp), m_ctx(ctx), m_root(nullptr) {}

  /**
   * @brief Destructor that clears the Set.
   */
  ~Set() {
    clear();
  }

  /*
    SetIterator class.
    Provides an iterator for traversing the binary search tree.
  */
  class SetIterator {
  private:
    TreeNode* m_node; // Pointer to the current node in the tree.

  public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    /**
     * @brief Constructor for the iterator.
     * @param node Pointer to a TreeNode to initialize the iterator.
     */
    SetIterator(TreeNode* node) : m_node(node) {}

    /**
     * @brief Dereference operator.
     * Accesses the value stored in the current node.
     * @return Reference to the stored value.
     */
    reference operator*() const {
      return m_node->m_data;
    }

    /**
     * @brief Arrow operator.
     * Provides access to the value stored in the current node.
     * @return Pointer to the stored value.
     */
    pointer operator->() const {
      return &(m_node->m_data);
    }

    /**
     * @brief Moves the iterator to the left child.
     * @return Iterator pointing to the left child.
     */
    SetIterator left() {
      return SetIterator(m_node->m_left);
    }

    /**
     * @brief Moves the iterator to the right child.
     * @return Iterator pointing to the right child.
     */
    SetIterator right() {
      return SetIterator(m_node->m_right);
    }

    /**
     * @brief Checks if two iterators point to the same node.
     * @param other Another iterator to compare.
     * @return True if both iterators are equal.
     */
    bool operator==(const SetIterator& other) const {
      return m_node == other.m_node;
    }

    /**
     * @brief Checks if two iterators point to different nodes.
     * @param other Another iterator to compare.
     * @return True if the iterators are different.
     */
    bool operator!=(const SetIterator& other) const {
      return m_node != other.m_node;
    }

    friend class Set;
  };

  /**
   * @brief Clears all elements from the set.
   */
  void clear() {
    clear_internal(m_root);
    m_root = nullptr;
  }

  /**
   * @brief Inserts a new element into the set.
   * @param data The value to insert.
   */
  void insert(const T& data) {
    if (!m_root) {
      m_root = new TreeNode(data);
    } else {
      TreeNode* curr = m_root;
      while (1) {
        int comp = m_cmp(curr->m_bucket.front(), data, m_ctx);
        ++curr->m_rank;
        if (comp < 0) { // curr->data < data
          if (!curr->m_right) {
            curr->m_right = new TreeNode(data);
            break;
          }
          curr = curr->m_right;
        } else if (comp > 0) { // curr->data > data
          if (!curr->m_left) {
            curr->m_left = new TreeNode(data);
            break;
          }
          curr = curr->m_left;
        } else { // curr->data == data
          curr->m_bucket.push_back(data);
          break;
        }
      }
      rebalance();
    }
  }

  /**
   * @brief Finds the first element that is not less than (i.e., greater or
   * equal to) a given value.
   * @param data The value to search for.
   * @param cmp Comparator function (default is less<T>).
   * @return Iterator pointing to the found element or the first greater
   * element.
   */
  template <typename T2, typename CMP2>
  SetIterator lower_bound(const T2& data, CMP2 cmp = CMP()) {
    if (!m_root)
      return end();

    TreeNode* curr = m_root;

    while (1) {
      int comp = cmp(curr->m_bucket.front(), data, m_ctx);
      if (comp < 0) { // curr->data < data
        if (!curr->m_right)
          return SetIterator(curr);
        curr = curr->m_right;
      } else if (comp > 0) { // curr->data > data
        if (!curr->m_left)
          return SetIterator(curr);
        curr = curr->m_left;
      } else { // curr->data == data
        return SetIterator(curr);
      }
    }
  }

  /**
   * @brief Returns an iterator pointing to the beginning of the set.
   * @return An iterator pointing to the first element.
   */
  SetIterator begin() {
    return SetIterator(m_root);
  }

  /**
   * @brief Returns an iterator pointing past the last element of the set.
   * @return An iterator pointing past the last element.
   */
  SetIterator end() {
    return SetIterator(nullptr);
  }

  /**
   * @brief Finds a specific element in the set.
   * @param data The value to search for.
   * @param cmp Comparator function (default is less<T>).
   * @return Iterator pointing to the element if found, otherwise end().
   */
  template <typename T2>
  SetIterator find(const T2& data, CMP cmp = CMP()) {
    T2 ret = *lower_bound(data, cmp);
    return ret == data ? ret : end();
  }

  /**
   * @brief Checks if the set is empty.
   * @return True if the set is empty, false otherwise.
   */
  bool empty() {
    return !m_root;
  }
};

#endif // SET
