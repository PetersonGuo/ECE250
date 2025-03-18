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

    /*
      Returns a reference to the stored data.
      @return Reference to the node's data.
    */
    T& data() {
      return m_data;
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
    Returns a const reference to the root node pointer.
    @return Const reference to the root node.
  */
  TreeNode* root() const {
    return m_root;
  }
};

#endif