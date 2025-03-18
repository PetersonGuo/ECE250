#include "../include/country_container.hpp"
#include "../data_structures/include/file.hpp"

#include <fstream>
#include <iostream>
#include <cfloat>
#include <cmath>

// Constructor initializes an empty CountryContainer with size 0.
CountryContainer::CountryContainer()
    : m_size(0), m_hash(m_data), m_data(m_hash) {
  m_data.set_capacity(512);
}

const Country& CountryContainer::operator[](std::string& key) {
  return m_data.at(key);
}

Country& CountryContainer::operator[](int index) {
  return (*m_data[index]).getSecond();
}

bool CountryContainer::contains(const std::string& key) const {
  return m_data.contains(key);
}

// Loads country data from a file.
void CountryContainer::load(const std::string& filename) {
  std::ifstream is;
  std::stringstream ss;
  std::string line, name, code;
  int index = 0;
  is.open(filename);

  if (is.fail())
    std::cout << "invalid file" << std::endl;

  // Read country data until EOF
  while (!is.eof()) {
    peek_line(is, line);
    ss.str(line);
    std::getline(ss, name, ',');
    std::getline(ss, code, ',');

    m_data[code].load(is);
    ++index;
  }

  m_size = index;
  is.close();
}

// Lists all time-series data for a specific country.
void CountryContainer::list(const std::string& countryName) const {
  for (auto i : m_data) {
    if (i.getSecond().get_country_name() == countryName) {
      i.getSecond().list();
      break;
    }
  }
}

// Computes the min and max mean values across all countries for a given series.
Pair<double, double>
CountryContainer::range(const std::string& seriesCode) const {
  double min = DBL_MAX, max = DBL_MIN;

  for (auto i : m_data) {
    TimeSeries& series = i.getSecond()[seriesCode];

    if (!series.invalid()) {
      double mean = series.mean();
      min = fmin(min, mean);
      max = fmax(max, mean);
    }
  }

  return {min, max};
}

inline double CountryContainer::mid_range(const RangeContainer& range) const {
  auto pair = range.get_range();
  return ((*pair.getSecond())[m_seriesCode].mean() +
          (*pair.getFirst())[m_seriesCode].mean()) /
         2.0;
}

inline int CountryContainer::CountryContainerHash::hash2(unsigned int W) const {
  unsigned int f = floor(W / (double)512);
  return f % 512 + (~f & 1);
}

inline int CountryContainer::CountryContainerHash::hash1(unsigned int W) const {
  return W % 512;
}

int CountryContainer::CountryContainerHash::hash(const std::string& countryCode,
                                                 unsigned int* comps) const {
  const unsigned int m = 512;
  unsigned int W = 0;
  unsigned int n = countryCode.size();
  for (unsigned int i = 0; i < n; ++i) {
    W = W * 26 + (countryCode[i] - 'A');
  }

  unsigned int i = 0, key;
  unsigned int firstAvailable = m;
  while (true) {
    key = (hash1(W) + i * hash2(W)) % m;
    ++i;

    if (!m_data[key]->used()) {
      if (firstAvailable == m)
        firstAvailable = key;
      break;
    } else {
      if (!m_data[key]->empty() &&
          (*m_data[key]).getSecond().get_country_code() == countryCode) {
        firstAvailable = m;
        break;
      } else if (m_data[key]->empty() && firstAvailable == m) {
        firstAvailable = key;
      }
    }
  }

  if (comps)
    *comps = i;

  return firstAvailable != m ? firstAvailable : key;
}

int CountryContainer::CountryContainerHash::operator()(
    const std::string& countryCode) const {
  return hash(countryCode, nullptr);
}

// Builds a balanced binary tree of countries sorted by mean values.
void CountryContainer::build(const std::string& seriesCode) {
  m_tree.clear();
  m_seriesCode = seriesCode;

  build_helper(-DBL_MAX, DBL_MAX, m_tree.root());
}

// Recursively builds the balanced tree.
void CountryContainer::build_helper(double rangeMin, double rangeMax,
                                    Tree<RangeContainer>::TreeNode*& parent) {
  RangeContainer container;
  Country *minCountry = nullptr, *maxCountry = nullptr;

  // Iterate by reference and only consider countries in the [rangeMin,
  // rangeMax] interval.
  for (auto item : m_data) {
    double mean = item.getSecond()[m_seriesCode].mean();
    if (mean >= rangeMin && mean < rangeMax) {
      container.get_countries().push_back(&item.getSecond());
      // Update min and max within the filtered set.
      if (!minCountry || mean < (*minCountry)[m_seriesCode].mean())
        minCountry = &item.getSecond();
      if (!maxCountry || mean > (*maxCountry)[m_seriesCode].mean())
        maxCountry = &item.getSecond();
    }
  }

  // If no country falls into the range, nothing to do.
  if (container.get_countries().empty())
    return;

  // Set the range in the container using the filtered countries.
  container.get_range() = {minCountry, maxCountry};
  double currentMin = (*minCountry)[m_seriesCode].mean();
  double currentMax = (*maxCountry)[m_seriesCode].mean();
  double currentMid = mid_range(container);

  // Create the current node.
  Tree<RangeContainer>::TreeNode* current =
      new Tree<RangeContainer>::TreeNode(container);

  // Attach the new node to the parent's left or right child.
  if (parent) {
    double parentMid = mid_range(parent->data());
    if (currentMid < parentMid)
      parent->left() = current;
    else
      parent->right() = current;
  } else {
    parent = current;
  }

  // Termination condition: if the range is already narrow enough.
  if (fabs(currentMax - currentMin) <= 1e-3)
    return;

  // Recursively build left and right subtrees.
  build_helper(currentMin, currentMid, current);
  build_helper(currentMid, currentMax + 1e-4, current);
}

// Prints a list of countries.
void CountryContainer::print_list(const List<Country*>& list) const {
  for (List<Country*>::Iterator it = list.begin(); it != list.end(); ++it)
    std::cout << (*it)->get_country_name() << ' ';
}

// Searches for countries based on mean value comparisons.
void CountryContainer::find(const double target, const char op) const {
  if (!m_tree.root()) {
    std::cout << "failure\n";
    return;
  }

  for (List<Country*>::Iterator it =
           m_tree.root()->data().get_countries().begin();
       it != m_tree.root()->data().get_countries().end(); ++it) {
    Country* country = *it;
    double mean = (*country)[m_seriesCode].mean();

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
void CountryContainer::limits(bool condition) const {
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

Pair<int, unsigned int>
CountryContainer::lookup(std::string& countryCode) const {
  unsigned int comp = 0;
  return {m_hash.hash(countryCode, &comp), comp};
}

bool CountryContainer::remove(std::string& countryCode) {
  if (!m_data.contains(countryCode) ||
      m_data.at(countryCode).get_country_code() != countryCode)
    return false;
  delete_country(m_data[countryCode].get_country_name());
  m_data.erase(countryCode);
  return true;
}

bool CountryContainer::insert(std::string& countryCode, std::string& filename) {
  if (m_data.contains(countryCode) &&
      m_data.at(countryCode).get_country_code() == countryCode)
    return false;

  std::ifstream is;
  std::stringstream ss;

  is.open(filename);

  if (is.fail())
    std::cout << "invalid file" << std::endl;

  // Read country data until EOF
  std::string line, code;
  while (!is.eof()) {
    peek_line(is, line);
    ss.str(line);
    std::getline(ss, code, ',');
    std::getline(ss, code, ',');
    if (code == countryCode)
      break;
    std::getline(is, line);
  }

  m_data[countryCode].load(is);

  is.close();
  return true;
}
