#ifndef BST
#define BST

/*
  A generic binary search tree (BST) class template.
  This is used internally for managing the RangeContainer objects.
*/
template <typename T>
class Tree {
public:
  /*
    Represents a single node in the Tree.
    Each node stores a data value and has left and right children.
  */
  class TreeNode {
  private:
    T m_data;                   // The data value stored in the node.
    TreeNode *m_left, *m_right; // Pointers to left and right child nodes.

  public:
    /*
      Default constructor initializes a node with null children.
    */
    TreeNode() : m_left(nullptr), m_right(nullptr) {}

    /*
      Parameterized constructor initializes a node with a given data value.
      @param data The value to store in the node.
    */
    TreeNode(T data) : m_data(data), m_left(nullptr), m_right(nullptr) {}

    T& operator*() {
      return m_data;
    }

    /*
      Returns a reference to the left child node.
      @return Reference to the left child pointer.
    */
    TreeNode*& left() {
      return m_left;
    }

    /*
      Returns a reference to the right child node.
      @return Reference to the right child pointer.
    */
    TreeNode*& right() {
      return m_right;
    }

    friend class Tree;     // Allows the Tree class to access private members.
    friend class Iterator; // Allows the Iterator class to access private
                           // members.
  };

  class Iterator {
  private:
    TreeNode* m_node; // Pointer to the current node in the tree.

    Iterator() : m_node(nullptr) {}

    Iterator(TreeNode* node) : m_node(node) {}

  public:
    /*
      Dereference operator returns a reference to the data value.
      @return Reference to the data value.
    */
    T& operator*() {
      return m_node->m_data;
    }

    T* operator->() {
      return &m_node->m_data;
    }

    Iterator left() {
      return Iterator(m_node->m_left);
    }

    Iterator right() {
      return Iterator(m_node->m_right);
    }

    /*
      Equality operator checks if two iterators are equal.
      @param other The iterator to compare against.
      @return True if the iterators are equal, false otherwise.
    */
    bool operator==(const Iterator& other) const {
      return m_node == other.m_node;
    }

    /*
      Inequality operator checks if two iterators are not equal.
      @param other The iterator to compare against.
      @return True if the iterators are not equal, false otherwise.
    */
    bool operator!=(const Iterator& other) const {
      return m_node != other.m_node;
    }

    friend class Tree; // Allows the Tree class to access private members.
  };

private:
  TreeNode* m_root; // Pointer to the root node of the tree.

  /*
    Recursively deletes all nodes in the tree starting from the given root.
    @param root The root node to begin clearing from.
  */
  void clear_internal(TreeNode* root) {
    if (!root)
      return;
    clear_internal(root->m_left);
    clear_internal(root->m_right);
    delete root;
  }

public:
  /*
    Default constructor initializes an empty tree.
  */
  Tree() : m_root(nullptr) {}

  /*
    Destructor clears the entire tree.
  */
  ~Tree() {
    clear();
  }

  /*
    Deletes all nodes from the tree and resets the root to nullptr.
  */
  void clear() {
    clear_internal(m_root);
    m_root = nullptr;
  }

  /*
    Returns a reference to the root node pointer.
    @return Reference to the root node.
  */
  TreeNode*& root() {
    return m_root;
  }

  /*
    Returns an iterator to the root node.
    @return Iterator to the root node.
  */
  Iterator begin() {
    return Iterator(m_root);
  }

  /*
    Returns an iterator to the end of the tree.
    @return Iterator to the end of the tree.
  */
  Iterator end() {
    return Iterator(nullptr);
  }

  Iterator end() const {
    return Iterator(nullptr);
  }

  Iterator begin() const {
    return Iterator(m_root);
  }

  void insert(const T& data) {
    TreeNode* node = new TreeNode(data);
    TreeNode* parent = nullptr;
    TreeNode* current = m_root;

    while (current) {
      parent = current;
      if (data < current->m_data)
        current = current->m_left;
      else
        current = current->m_right;
    }

    if (!parent)
      m_root = node;
    else if (data < parent->m_data)
      parent->m_left = node;
    else
      parent->m_right = node;
  }
};

#endif