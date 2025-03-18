#ifndef COUNTRY_CONTAINER
#define COUNTRY_CONTAINER
#include "country.hpp"
#include "../data_structures/include/list.hpp"
#include <string>

/*
  The CountryContainer class is responsible for managing multiple Country
  objects. It provides functionality for loading, listing, searching, and
  deleting countries, while using a balanced binary tree structure for optimized
  storage and retrieval.
*/
class CountryContainer {
private:
  /*
    The RangeContainer class represents a grouping of Country objects.
    It maintains a list of Country pointers along with the minimum and
    maximum Country objects in the range.
  */
  class RangeContainer {
  private:
    Country *m_min,
        *m_max; // Pointers to the minimum and maximum countries in the range.
    List<Country*> m_countries; // List of countries within the range.

  public:
    /*
      Returns a pair containing references to the minimum and maximum Country
      objects.
      @return A Pair object containing m_min and m_max.
    */
    Pair<Country*&, Country*&> get_range() {
      return Pair<Country*&, Country*&>(m_min, m_max);
    }

    /*
      Returns the list of Country pointers stored in this RangeContainer.
      @return Reference to the list of countries.
    */
    List<Country*>& get_countries() {
      return m_countries;
    }
  };

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
  };

  Country m_data[512]; // Fixed-size array to store country data.
  Tree<RangeContainer>
      m_tree;               // Binary tree used for range-based country storage.
  std::size_t m_size;       // Number of countries stored.
  std::string m_seriesCode; // Series code for identifying data.

  /*
    Prints the list of countries stored in the provided List object.
    @param list The List object containing Country pointers to print.
  */
  void print_list(const List<Country*>& list);

  /*
    Recursively builds the tree structure from country data.
    @param start Start index for the range.
    @param end End index for the range.
    @param node Reference to the tree node being constructed.
  */
  void build_helper(int start, int end, Tree<RangeContainer>::TreeNode*& node);

  /*
    Computes the mid-range value for a given RangeContainer.
    @param range The RangeContainer object to compute the mid-range for.
    @return The mid-range value.
  */
  double mid_range(RangeContainer& range);

  /*
    Helper function for deleting a country from the tree.
    @param node Pointer to the tree node being checked for deletion.
    @param countryName The name of the country to delete.
    @return True if the country was successfully deleted, false otherwise.
  */
  bool delete_country_helper(Tree<RangeContainer>::TreeNode* node,
                             const std::string& countryName);

public:
  /*
    Default constructor initializes an empty CountryContainer.
  */
  CountryContainer();

  /*
    Loads country data from a file.
    @param filename The name of the file to read data from.
  */
  void load(const std::string& filename);

  /*
    Lists all countries stored in the container.
    @param seriesCode The code used to filter which series to list.
  */
  void list(const std::string& seriesCode);

  /*
    Computes the range of values for a given series.
    @param seriesCode The code used to determine which series to analyze.
    @return A Pair containing the minimum and maximum values in the range.
  */
  Pair<double, double> range(const std::string& seriesCode);

  /*
    Builds the tree structure based on country data.
    @param seriesCode The code used to determine which series to use for
    building.
  */
  void build(const std::string& seriesCode);

  /*
    Finds and prints all countries that match a given condition.
    @param value The threshold value to compare against.
    @param condition The condition character ('<' or '>') to filter results.
  */
  void find(const double value, const char condition);

  /*
    Deletes a country from the container.
    @param countryName The name of the country to delete.
    @return True if the country was successfully deleted, false otherwise.
  */
  bool delete_country(const std::string& countryName);

  /*
    Finds the min/max means in the BST
    @param condition Boolean flag indicating if looking for min
  */
  void limits(bool condition);
};
#endif
