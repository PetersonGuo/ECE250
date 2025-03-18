#include "../include/country_container.hpp"
#include "../data_structures/include/algorithm.hpp"

#include <fstream>
#include <iostream>
#include <cfloat>
#include <cmath>

// Constructor initializes an empty CountryContainer with size 0.
CountryContainer::CountryContainer() : m_size(0) {}

// Loads country data from a file.
void CountryContainer::load(const std::string& fileName) {
  std::ifstream is;
  int index = 0;
  is.open(fileName);

  if (is.fail()) {
    std::cout << "invalid file" << std::endl;
  }

  // Read country data until EOF
  while (!is.eof()) {
    m_data[index].load(is);
    ++index;
  }

  m_size = index;
  is.close();
}

// Lists all time-series data for a specific country.
void CountryContainer::list(const std::string& countryName) {
  for (std::size_t i = 0; i < m_size; ++i) {
    if (m_data[i].get_country_name() == countryName) {
      m_data[i].list();
      break;
    }
  }
}

// Computes the min and max mean values across all countries for a given series.
Pair<double, double> CountryContainer::range(const std::string& seriesCode) {
  double min = DBL_MAX, max = DBL_MIN;

  for (std::size_t i = 0; i < m_size; ++i) {
    List<TimeSeries>::ListIterator series = m_data[i].find_series(seriesCode);

    if (!series->invalid()) {
      double mean = series->mean();
      min = fmin(min, mean);
      max = fmax(max, mean);
    }
  }

  return {min, max};
}

// Computes the mid-range value for a given range container.
double CountryContainer::mid_range(RangeContainer& range) {
  auto pair = range.get_range();
  return (pair.getSecond()->find_series(m_seriesCode)->mean() +
          pair.getFirst()->find_series(m_seriesCode)->mean()) /
         2.0;
}

// Builds a balanced binary tree of countries sorted by mean values.
void CountryContainer::build(const std::string& seriesCode) {
  m_tree.clear();
  m_seriesCode = seriesCode;

  // Sorts countries based on the mean value of the given series.
  sort(m_data, m_data + m_size, [this](const Country& a, const Country& b) {
    return a.find_series(this->m_seriesCode)->mean() <
           b.find_series(this->m_seriesCode)->mean();
  });

  build_helper(0, m_size - 1, m_tree.root());
}

// Recursively builds the balanced tree.
void CountryContainer::build_helper(int left, int right,
                                    Tree<RangeContainer>::TreeNode*& parent) {
  if (left > right)
    return;

  // Compute left and right mean values.
  double leftMean = m_data[left].find_series(m_seriesCode)->mean();
  double rightMean = m_data[right].find_series(m_seriesCode)->mean();

  RangeContainer container;
  for (int i = left; i <= right; ++i)
    container.get_countries().push_back(&m_data[i]);

  auto range = container.get_range();
  range.getFirst() = &m_data[left];
  range.getSecond() = &m_data[right];

  Tree<RangeContainer>::TreeNode* current =
      new Tree<RangeContainer>::TreeNode(container);

  // Attach to the appropriate parent node.
  if (parent) {
    double parentMid = mid_range(parent->data());
    (parentMid >= leftMean ? parent->left() : parent->right()) = current;
  } else {
    parent = current;
  }

  // Base case: If left and right means are nearly identical, stop recursion.
  if (fabs(leftMean - rightMean) <= 1e-3)
    return;

  // Find partition index based on mid value.
  double threshold = (leftMean + rightMean) / 2.0;
  int partitionIndex = right + 1;

  for (int i = left; i <= right; ++i) {
    double m = 0.0;
    auto series = m_data[i].find_series(m_seriesCode);
    if (series != m_data[i].end())
      m = series->mean();
    if (m >= threshold) {
      partitionIndex = i;
      break;
    }
  }

  if (partitionIndex == left && left < right)
    partitionIndex = left + 1;

  build_helper(left, partitionIndex - 1, current);
  build_helper(partitionIndex, right, current);
}

// Prints a list of countries.
void CountryContainer::print_list(const List<Country*>& list) {
  for (List<Country*>::ListIterator it = list.begin(); it != list.end(); ++it)
    std::cout << (*it)->get_country_name() << ' ';
}

// Searches for countries based on mean value comparisons.
void CountryContainer::find(const double target, const char op) {
  if (!m_tree.root()) {
    std::cout << "failure\n";
    return;
  }

  for (List<Country*>::ListIterator it =
           m_tree.root()->data().get_countries().begin();
       it != m_tree.root()->data().get_countries().end(); ++it) {
    Country* country = *it;
    double mean = country->find_series(m_seriesCode)->mean();

    if (op == -1 && mean < target) {
      std::cout << country->get_country_name() << " ";
    } else if (op == 0 && fabs(mean - target) <= 1e-3) {
      std::cout << country->get_country_name() << " ";
    } else if (op == 1 && mean > target) {
      std::cout << country->get_country_name() << " ";
    }
  }
  std::cout << std::endl;
}

// Deletes a country from the data structure.
bool CountryContainer::delete_country(const std::string& countryName) {
  if (!m_tree.root())
    return false;
  return delete_country_helper(m_tree.root(), countryName);
}

// Helper function for recursively deleting a country.
bool CountryContainer::delete_country_helper(
    Tree<RangeContainer>::TreeNode* node, const std::string& countryName) {
  if (!node)
    return false;

  List<Country*>& bucket = node->data().get_countries();

  for (auto it = bucket.begin(); it != bucket.end(); ++it) {
    if ((*it)->get_country_name() == countryName) {
      bucket.erase(it);
      return true;
    }
  }

  return delete_country_helper(node->left(), countryName) ||
         delete_country_helper(node->right(), countryName);
}

// Finds and prints the lowest or highest value node in the tree.
void CountryContainer::limits(bool condition) {
  Tree<RangeContainer>::TreeNode *root = m_tree.root(), *prev = nullptr;

  // Traverse left for min, right for max
  if (condition) {
    while (root) {
      prev = root;
      root = root->left();
    }
  } else {
    while (root) {
      prev = root;
      root = root->right();
    }
  }

  if (!prev)
    std::cout << "failure";
  else
    print_list(prev->data().get_countries());
  putchar('\n');
}
