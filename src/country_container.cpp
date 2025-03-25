#include "../include/country_container.hpp"
#include "../include/file.hpp"
#include "../include/algorithm.hpp"

#include <queue>
#include <cassert>
#include <fstream>
#include <iostream>
#include <cfloat>
#include <cmath>

using vec_iter = std::vector<std::string>::const_iterator;

inline std::ostream&
operator<<(std::ostream& out,
           const std::tuple<std::string, double, RELATION>& tup) {
  return out << '(' << std::get<0>(tup) << ' ' << std::get<1>(tup) << ' '
             << std::get<2>(tup) << ')';
}

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec) {
  for (size_t i = 0; i < vec.size(); ++i)
    out << vec[i] << (i == vec.size() - 1 ? "" : " ");
  out << std::endl;
  return out;
}

inline double CountryContainer::mid_range(const RangeContainer& range) const {
  std::pair<std::string, std::string> pair = range.get_range();
  return (m_data.at(pair.second)[m_seriesCode].mean() -
          m_data.at(pair.first)[m_seriesCode].mean()) /
             2.0 +
         m_data.at(pair.first)[m_seriesCode].mean();
}

void CountryContainer::initialize() {
  m_graph.clear();
  for (const std::string& i : m_countryCodes)
    m_graph.add_node(i);
}

bool CountryContainer::update_edges(const std::string& seriesCode,
                                    double threshold, RELATION relation) {
  bool fail = 1;
  build(seriesCode);
  const std::vector<std::string> countries = find(threshold, relation);
  for (vec_iter i = countries.begin(); i != countries.end(); ++i)
    for (vec_iter j = i + 1; j != countries.end(); ++j)
      if (m_graph.add_edge(*i, *j, {seriesCode, threshold, relation}) &&
          m_graph.add_edge(*j, *i, {seriesCode, threshold, relation}))
        fail = 0;
  return !fail;
}

void CountryContainer::adjacent(const std::string& countryCode) {
  if (!m_graph.has_node(countryCode))
    std::cout << "failure" << std::endl;
  else if (!m_graph.get_edges(countryCode).size())
    std::cout << "none" << std::endl;
  else {
    std::vector<std::string> countries;
    for (const std::pair<const std::string, Graph::relations_set>& i :
         m_graph.get_edges(countryCode))
      countries.push_back(m_data.at(i.first).get_country_name());
    std::cout << countries;
  }
}

bool CountryContainer::path(const std::string& countryCode1,
                            const std::string& countryCode2) {
  std::unordered_set<std::string> visited;
  std::queue<std::string> q;
  q.push(countryCode1);
  visited.insert(countryCode1);
  while (!q.empty()) {
    std::string current = q.front();
    q.pop();
    for (const std::pair<const std::string, Graph::relations_set>& i :
         m_graph.get_edges(current)) {
      if (i.first == countryCode2)
        return true;
      if (!visited.count(i.first)) {
        visited.insert(i.first);
        q.push(i.first);
      }
    }
  }
  return false;
}

void CountryContainer::relationships(const std::string& countryCode1,
                                     const std::string& countryCode2) {
  if (!m_graph.has_edge(countryCode1, countryCode2))
    std::cout << "none" << std::endl;
  else {
    const Graph::relations_vector& relations =
        m_graph.get_relations_ordered(countryCode1, countryCode2);
    for (const std::tuple<std::string, double, RELATION>& i : relations)
      std::cout << "(" << std::get<0>(i) << " " << std::get<1>(i) << " "
                << std::get<2>(i) << ") ";
    std::cout << std::endl;
  }
}

// Loads country data from a file.
void CountryContainer::load(const std::string& filename) {
  std::ifstream is;
  std::stringstream ss;
  std::string line, code;
  bool ret = 1;

  is.open(filename);

  if (is.fail())
    std::cout << "invalid file" << std::endl;

  // Read country data until EOF
  while (ret) {
    peekline(is, line);
    ss.str(line);
    std::getline(ss, line, ',');
    std::getline(ss, code, ',');

    ret = m_data[code].load(is);
    m_countryCodes.push_back(code);
  }

  is.close();
}

void CountryContainer::print() {
  std::queue<std::pair<Tree<RangeContainer>::Iterator, int>> q;
  q.push({m_tree.begin(), 0});
  while (!q.empty()) {
    auto [node, rank] = q.front();
    q.pop();
    if (node != m_tree.end()) {
      std::cout << "Range: " << node->get_range().first << " "
                << m_data.at(node->get_range().first)[m_seriesCode].mean()
                << " " << node->get_range().second << " "
                << m_data.at(node->get_range().second)[m_seriesCode].mean()
                << " " << rank << std::endl;
      q.push({node.left(), rank + 1});
      q.push({node.right(), rank + 1});
    }
  }
}

// Builds a binary tree of countries sorted by mean values.
void CountryContainer::build(const std::string& seriesCode) {
  m_tree.clear();
  m_seriesCode = seriesCode;
  algorithm::sort(m_countryCodes.begin(), m_countryCodes.end(),
                  [this](const std::string& a, const std::string& b) {
                    return m_data.at(a)[m_seriesCode].mean() <
                           m_data.at(b)[m_seriesCode].mean();
                  });
  m_tree.root() = build_helper(0, m_countryCodes.size() - 1, m_countryCodes);
}

// Recursively builds the tree.
Tree<CountryContainer::RangeContainer>::TreeNode*
CountryContainer::build_helper(int left, int right,
                               const std::vector<std::string>& data) {
  if (left > right)
    return nullptr;

  RangeContainer container(data[left], data[right], data.begin() + left,
                           data.begin() + right + 1);

  long mid =
      algorithm::lower_bound(data.begin() + left, data.begin() + right + 1,
                             mid_range(container),
                             [this](const std::string& a, const double b) {
                               return m_data.at(a)[m_seriesCode].mean() < b;
                             }) -
      data.begin();

  // Create the current node.
  Tree<RangeContainer>::TreeNode* current =
      new Tree<RangeContainer>::TreeNode(container);

  if (fabs(m_data.at(data[left])[m_seriesCode].mean() -
           m_data.at(data[right - 1])[m_seriesCode].mean()) <= 1e-3 ||
      left == right)
    return current;

  // Recursively build left and right subtrees.
  current->left() = build_helper(left, mid - 1, data);
  current->right() = build_helper(mid, right, data);

  return current;
}

// Searches for countries based on mean value comparisons.
std::vector<std::string> CountryContainer::find(const double target,
                                                const RELATION op) const {
  if (m_tree.begin() == m_tree.end())
    return {};

  std::vector<std::string> result;
  const std::vector<std::string>& data = m_tree.begin()->get_countries();
  for (RangeContainer::const_iterator it = data.begin(); it != data.end();
       ++it) {
    double mean = m_data.at(*it)[m_seriesCode].mean();

    if (fabs(mean - target) <= 1e-3) {
      if (op == EQUAL)
        result.push_back(*it);
    } else {
      if (op == LESS && mean < target)
        result.push_back(*it);
      else if (op == GREATER && mean > target)
        result.push_back(*it);
    }
  }
  return result;
}
