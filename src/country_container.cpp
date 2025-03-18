#include "../include/country_container.hpp"
#include "../data_structures/include/file.hpp"
#include "../data_structures/include/queue.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <cfloat>
#include <cmath>

inline double CountryContainer::mid_range(const RangeContainer& range) const {
  auto pair = range.get_range();
  return (m_data.at(pair.getSecond())[m_seriesCode].mean() -
          m_data.at(pair.getFirst())[m_seriesCode].mean()) /
             2.0 +
         m_data.at(pair.getFirst())[m_seriesCode].mean();
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
      if (!m_data[key]->empty() && (*m_data[key]).getFirst() == countryCode) {
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

// Prints a list of countries.
void CountryContainer::print_list(const List<std::string>& list) const {
  for (List<std::string>::Iterator it = list.begin(); it != list.end(); ++it)
    std::cout << m_data.at(*it).get_country_name() << ' ';
}

// Constructor initializes an empty CountryContainer with size 0.
CountryContainer::CountryContainer()
    : m_size(0), m_hash(m_data), m_data(m_hash) {
  m_data.set_capacity(512);
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
  for (auto& i : m_data) {
    if (i.getSecond().get_country_name() == countryName) {
      i.getSecond().list();
      break;
    }
  }
}

void CountryContainer::print() {
  Queue<Tree<RangeContainer>::TreeNode*> q;
  q.push(m_tree.root());
  while (!q.empty()) {
    Tree<RangeContainer>::TreeNode* node = q.pop();
    if (node) {
      std::cout
          << "Range: " << node->data().get_range().getFirst() << " "
          << m_data.at(node->data().get_range().getFirst())[m_seriesCode].mean()
          << " " << node->data().get_range().getSecond() << " "
          << m_data.at(node->data().get_range().getSecond())[m_seriesCode]
                 .mean()
          << std::endl;
      q.push(node->left());
      q.push(node->right());
    }
  }
}

// Computes the min and max mean values across all countries for a given series.
Pair<double, double>
CountryContainer::range(const std::string& seriesCode) const {
  double min = DBL_MAX, max = DBL_MIN;

  for (auto& i : m_data) {
    TimeSeries& series = i.getSecond()[seriesCode];

    if (!series.invalid()) {
      double mean = series.mean();
      min = fmin(min, mean);
      max = fmax(max, mean);
    }
  }

  return {min, max};
}

// Builds a binary tree of countries sorted by mean values.
void CountryContainer::build(const std::string& seriesCode) {
  m_tree.clear();
  m_seriesCode = seriesCode;
  m_tree.root() = build_helper(-DBL_MAX, DBL_MAX);
}

// Recursively builds the tree.
Tree<CountryContainer::RangeContainer>::TreeNode*
CountryContainer::build_helper(double rangeMin, double rangeMax) {
  if (fabs(rangeMin - rangeMax) <= 1e-3)
    return nullptr;

  RangeContainer container;
  std::string min, max;

  // Iterate by reference and only consider countries in the [rangeMin,
  // rangeMax] interval.
  for (Pair<std::string, Country>& item : m_data) {
    double mean = item.getSecond()[m_seriesCode].mean();
    if (rangeMin <= mean && mean < rangeMax) {
      container.get_countries().push_back(item.getFirst());
      // Update min and max within the filtered set.
      if (min == "" || mean < m_data.at(min)[m_seriesCode].mean())
        min = item.getFirst();
      if (max == "" || mean > m_data.at(max)[m_seriesCode].mean())
        max = item.getFirst();
    }
  }

  assert(min != "" && "Min should not be empty");
  assert(max != "" && "Max should not be empty");

  // Set the range in the container using the filtered countries.
  double currentMin = m_data.at(min)[m_seriesCode].mean();
  double currentMax = m_data.at(max)[m_seriesCode].mean();

  container.set_range(min, max);
  double currentMid = mid_range(container);

  // If no country falls into the range, nothing to do.
  if (container.get_countries().empty())
    return nullptr;

  // Create the current node.
  Tree<RangeContainer>::TreeNode* current =
      new Tree<RangeContainer>::TreeNode(container);

  // Recursively build left and right subtrees.
  current->left() = build_helper(currentMin, currentMid);
  current->right() = build_helper(currentMid, currentMax + 1e-6);

  return current;
}

// Searches for countries based on mean value comparisons.
void CountryContainer::find(const double target, const char op) const {
  if (!m_tree.root()) {
    std::cout << "failure\n";
    return;
  }

  List<std::string> data = m_tree.root()->data().get_countries();
  for (List<std::string>::Iterator it = data.begin(); it != data.end(); ++it) {
    std::string countryCode = *it;
    double mean = m_data.at(countryCode)[m_seriesCode].mean();

    if (fabs(mean - target) <= 1e-3) {
      if (op == 0)
        std::cout << m_data.at(countryCode).get_country_name() << " ";
    } else {
      if (op == -1 && mean < target) {
        std::cout << m_data.at(countryCode).get_country_name() << " ";
      } else if (op == 1 && mean > target) {
        std::cout << m_data.at(countryCode).get_country_name() << " ";
      }
    }
  }
  std::cout << std::endl;
}

// Deletes a country from the data structure.
bool CountryContainer::delete_country(const std::string& countryName) {
  if (!m_tree.root())
    return false;
  for (auto& i : m_data)
    if (i.getSecond().get_country_name() == countryName)
      return delete_country_helper(m_tree.root(),
                                   i.getSecond().get_country_code());
  return false;
}

// Helper function for recursively deleting a country.
bool CountryContainer::delete_country_helper(
    Tree<RangeContainer>::TreeNode* node, const std::string& countryCode) {
  if (!node)
    return false;

  List<std::string>& bucket = node->data().get_countries();
  for (auto it = bucket.begin(); it != bucket.end(); ++it) {
    if (*it == countryCode) {
      bucket.erase(it);
      if (mid_range(node->data()) > m_data.at(countryCode)[m_seriesCode].mean())
        delete_country_helper(node->left(), countryCode);
      else
        delete_country_helper(node->right(), countryCode);
      return true;
    }
  }

  return false;
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
  if (!m_data.contains(countryCode))
    return false;
  delete_country(m_data.at(countryCode).get_country_name());
  m_data.erase(countryCode);
  return true;
}

bool CountryContainer::insert(std::string& countryCode, std::string& filename) {
  if (m_data.contains(countryCode))
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
