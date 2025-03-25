#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <cstdlib>
#include <vector>

#include "../include/relation.hpp"

class Graph {
private:
  class TupleHash {
  public:
    std::size_t
    operator()(std::tuple<std::string, double, RELATION> const& item) const {
      return std::hash<std::string>()(std::get<0>(item)) ^
             std::hash<double>()(std::get<1>(item)) ^
             std::hash<RELATION>()(std::get<2>(item));
    }
  };

public:
  using relations_set =
      std::unordered_set<std::tuple<std::string, double, RELATION>,
                         Graph::TupleHash>;
  using relations_vector =
      std::vector<std::tuple<std::string, double, RELATION>>;
  using edges = std::unordered_map<std::string, relations_set>;

  std::unordered_map<std::string, edges> m_graph;
  std::unordered_map<std::string,
                     std::unordered_map<std::string, relations_vector>>
      m_graphOrdered;

public:
  void add_node(const std::string&);
  bool add_edge(const std::string&, const std::string&,
                const std::tuple<std::string, double, RELATION>&);
  bool has_edge(const std::string&, const std::string&) const;
  bool has_node(const std::string&) const;
  const relations_set& get_relations(const std::string&,
                                     const std::string&) const;
  const relations_vector& get_relations_ordered(const std::string& node1,
                                                const std::string& node2) const;
  void remove_edge(const std::string&, const std::string&);
  void remove_node(const std::string&);
  void print_adjacent(const std::string&) const;
  const edges& get_edges(const std::string&) const;
  void clear();
};

#endif