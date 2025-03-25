#ifndef COUNTRY_CONTAINER
#define COUNTRY_CONTAINER
#include "country.hpp"
#include "../include/relation.hpp"
#include "../include/graph.hpp"
#include "bst.hpp"

#include <string>
#include <unordered_map>

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
    std::vector<std::string> m_countries; // List of countries within the range.

  public:
    using iterator = std::vector<std::string>::iterator;
    using const_iterator = std::vector<std::string>::const_iterator;

    /*
      Default constructor initializes an empty RangeContainer.
    */
    RangeContainer() = default;

    /*
      Parameterized constructor initializes a RangeContainer with a minimum and
      maximum country name, along with a list of countries.
      @param min The name of the minimum country.
      @param max The name of the maximum country.
      @param begin Iterator to the beginning of the country list.
      @param end Iterator to the end of the country list.
    */
    RangeContainer(const std::string& min, const std::string& max,
                   std::vector<std::string>::const_iterator begin,
                   std::vector<std::string>::const_iterator end)
        : m_min(min), m_max(max), m_countries(begin, end) {}

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
      @return A std::pair object containing m_min and m_max.
    */
    std::pair<std::string, std::string> get_range() const {
      return {m_min, m_max};
    }

    /*
      Returns the list of Country pointers stored in this RangeContainer.
      @return Reference to the list of countries.
    */
    std::vector<std::string>& get_countries() {
      return m_countries;
    }
  };

  /*
    Prints the tree of countries stored in the provided binary tree object.
  */
  void print();

  /*
    Builds the tree structure based on country data.
    @param seriesCode The code used to determine which series to use for
    building.
  */
  void build(const std::string&);

  /*
    Recursively builds the tree structure from country data.
    @param start Start index for the range.
    @param end End index for the range.
    @param node Reference to the tree node being constructed.
  */
  Tree<RangeContainer>::TreeNode*
  build_helper(int left, int right, const std::vector<std::string>& data);

  /*
    Computes the mid-range value for a given RangeContainer.
    @param range The RangeContainer object to compute the mid-range for.
    @return The mid-range value.
  */
  double mid_range(const RangeContainer& range) const;

  /*
    Finds and prints all countries that match a given condition.
    @param value The threshold value to compare against.
    @param condition The relation to filter results.
  */
  std::vector<std::string> find(const double, const RELATION) const;

  Tree<RangeContainer>
      m_tree;               // Binary tree used for range-based country storage.
  std::string m_seriesCode; // Series code for identifying data.

  std::unordered_map<std::string, Country> m_data; // Map to store country data.

  std::vector<std::string> m_countryCodes;
  Graph m_graph;

public:
  /*
    Default constructor initializes an empty CountryContainer.
  */
  CountryContainer() = default;

  void initialize();

  bool update_edges(const std::string&, double, RELATION);

  void adjacent(const std::string&);

  bool path(const std::string&, const std::string&);

  void relationships(const std::string&, const std::string&);

  /*
    Loads country data from a file.
    @param filename The name of the file to read data from.
  */
  void load(const std::string&);
};
#endif
