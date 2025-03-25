#include "../include/graph.hpp"

#include <iostream>

void Graph::add_node(const std::string& node) {
  if (!has_node(node))
    m_graph[node] = {};
}

bool Graph::add_edge(const std::string& node1, const std::string& node2,
                     const std::tuple<std::string, double, RELATION>& edge) {
  if (has_edge(node1, node2) && get_relations(node1, node2).count(edge))
    return false;
  m_graph[node1][node2].insert(edge);
  m_graphOrdered[node1][node2].push_back(edge);
  return true;
}

bool Graph::has_edge(const std::string& node1, const std::string& node2) const {
  return has_node(node1) && m_graph.at(node1).count(node2);
}

bool Graph::has_node(const std::string& node) const {
  return m_graph.count(node);
}

const Graph::relations_set& Graph::get_relations(const std::string& node1,
                                             const std::string& node2) const {
  return m_graph.at(node1).at(node2);
}

const Graph::relations_vector&
Graph::get_relations_ordered(const std::string& node1, const std::string& node2) const {
  return m_graphOrdered.at(node1).at(node2);
}

void Graph::print_adjacent(const std::string& node) const {
  for (const std::pair<std::string, relations_set> i : m_graph.at(node))
    std::cout << i.first << ' ';
  std::cout << std::endl;
}

const Graph::edges& Graph::get_edges(const std::string& node) const {
  return m_graph.at(node);
}

void Graph::clear() {
  m_graph.clear();
}