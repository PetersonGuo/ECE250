#ifndef COUNTRY_CONTAINER
#define COUNTRY_CONTAINER
#include "country.hpp"
#include "../data_structures/include/list.hpp"
#include "../data_structures/include/unordered_map.hpp"
#include "bst.hpp"
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
    std::string m_min, m_max;
    List<std::string> m_countries; // List of countries within the range.

  public:
    /*
      Sets the minimum and maximum Country objects for the range.
      @param min The name of the minimum country.
      @param max The name of the maximum country.
    */
    void set_range(const std::string& min, const std::string& max) {
      m_min = min;
      m_max = max;
    }

    /*
      Returns a pair containing references to the minimum and maximum Country
      objects.
      @return A Pair object containing m_min and m_max.
    */
    Pair<std::string, std::string> get_range() const {
      return {m_min, m_max};
    }

    /*
      Returns the list of Country pointers stored in this RangeContainer.
      @return Reference to the list of countries.
    */
    List<std::string>& get_countries() {
      return m_countries;
    }
  };

  /*
    The CountryContainerHash class is a custom hash function object for
    hashing country names.
  */
  class CountryContainerHash {
  private:
    Map<std::string, Country, CountryContainerHash>& m_data;

    /*
      Computes a hash value for a given integer.
      @param key The integer to hash.
      @return The computed hash value.
    */
    int hash2(unsigned int) const;

    /*
      Computes a hash value for a given integer.
      @param key The integer to hash.
      @return The computed hash value.
    */
    int hash1(unsigned int) const;

    /*
      Computes a hash value for a given string.
      @param countryCode The string to hash.
      @return The computed hash value.
    */
    int hash(const std::string&) const;

  public:
    /*
      Parameterized constructor initializes the hash function with a reference
      to the country data map.
      @param data Reference to the map of country data.
    */
    CountryContainerHash(Map<std::string, Country, CountryContainerHash>& data)
        : m_data(data) {}

    /*
      Computes a hash value for a given string.
      @param countryCode The string to hash.
      @return The computed hash value.
    */
    int operator()(const std::string&) const;

    /*
      Computes a hash value for a given string.
      @param countryCode The string to hash.
      @param comps Pointer to store the number of comparisons made.
      @return The computed hash value.
    */
    int hash(const std::string&, unsigned int*) const;
  };

  /*
    Prints the list of countries stored in the provided List object.
    @param list The List object containing Country pointers to print.
  */
  void print_list(const List<std::string>&) const;

  /*
    Prints the tree of countries stored in the provided binary tree object.
  */
  void print();

  /*
    Recursively builds the tree structure from country data.
    @param start Start index for the range.
    @param end End index for the range.
    @param node Reference to the tree node being constructed.
  */
  Tree<RangeContainer>::TreeNode* build_helper(double, double);

  /*
    Computes the mid-range value for a given RangeContainer.
    @param range The RangeContainer object to compute the mid-range for.
    @return The mid-range value.
  */
  double mid_range(const RangeContainer& range) const;

  /*
    Helper function for deleting a country from the tree.
    @param node Pointer to the tree node being checked for deletion.
    @param countryName The name of the country to delete.
    @return True if the country was successfully deleted, false otherwise.
  */
  bool delete_country_helper(Tree<RangeContainer>::TreeNode*,
                             const std::string&);

  static const unsigned int m_dataSize = 512;
  Tree<RangeContainer>
      m_tree;               // Binary tree used for range-based country storage.
  std::size_t m_size;       // Number of countries stored.
  std::string m_seriesCode; // Series code for identifying data.

  CountryContainerHash m_hash;

  Map<std::string, Country, CountryContainerHash>
      m_data; // Map to store country data.

public:
  /*
    Default constructor initializes an empty CountryContainer.
  */
  CountryContainer();

  /*
    Checks if a country with the given name exists in the container.
    @param countryName The name of the country to search for.
  */
  bool contains(const std::string&) const;

  /*
    Loads country data from a file.
    @param filename The name of the file to read data from.
  */
  void load(const std::string&);

  /*
    Lists all countries stored in the container.
    @param seriesCode The code used to filter which series to list.
  */
  void list(const std::string&) const;

  /*
    Computes the range of values for a given series.
    @param seriesCode The code used to determine which series to analyze.
    @return A Pair containing the minimum and maximum values in the range.
  */
  Pair<double, double> range(const std::string&) const;

  /*
    Builds the tree structure based on country data.
    @param seriesCode The code used to determine which series to use for
    building.
  */
  void build(const std::string&);

  /*
    Finds and prints all countries that match a given condition.
    @param value The threshold value to compare against.
    @param condition The condition character ('<' or '>') to filter results.
  */
  void find(const double, const char) const;

  /*
    Deletes a country from the container.
    @param countryName The name of the country to delete.
    @return True if the country was successfully deleted, false otherwise.
  */
  bool delete_country(const std::string&);

  /*
    Finds the min/max means in the BST
    @param condition Boolean flag indicating if looking for min
  */
  void limits(bool) const;

  /*
    Looks up a country in the container.
    @param countryCode The code of the country to search for.
    @return A Pair containing the index and number of searches.
  */
  Pair<int, unsigned int> lookup(std::string&) const;

  /*
    Removes a country from the container.
    @param countryCode The code of the country to remove.
    @return True if the country was successfully removed, false otherwise.
  */
  bool remove(std::string&);

  /*
    Inserts a new country into the container.
    @param countryCode The code of the country to insert.
    @param filename The name of the file containing the country data.
    @return True if the country was successfully inserted, false otherwise.
  */
  bool insert(std::string&, std::string&);
};
#endif
