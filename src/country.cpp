/*
  Implementation of the Country class.
  This file contains the function definitions for managing and performing
  calculations on TimeSeries lists, including largest mean calculations,
  deleting Time Series's, and loading TimeSeries for countries.
*/

#include "../include/country.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cfloat>

// Invalidate cache upon instantiation
Country::Country() {}

// Country.cpp
Country::Country(const Country& other)
    : m_name(other.m_name), m_code(other.m_code) {
  m_series = other.m_series;
}

Country& Country::operator=(const Country& other) {
  if (this != &other) {
    m_name = other.m_name;
    m_code = other.m_code;
    m_series = other.m_series;
  }
  return *this;
}

Country::Country(Country&& other) noexcept
    : m_name(std::move(other.m_name)), m_code(std::move(other.m_code)),
      m_series(std::move(other.m_series)) {}

Country& Country::operator=(Country&& other) noexcept {
  if (this != &other) {
    m_name = std::move(other.m_name);
    m_code = std::move(other.m_code);
    m_series = std::move(other.m_series);
  }
  return *this;
}

void Country::close() {
  m_name = "";
  m_code = "";
  m_series.clear();
}

List<TimeSeries>& Country::dummy_series_container() {
  static List<TimeSeries> dummyContainer;
  static bool initialized = false;
  if (!initialized) {
    TimeSeries dummySeries;
    dummyContainer.push_back(dummySeries);
    initialized = true;
  }
  return dummyContainer;
}

List<TimeSeries>& Country::dummy_series_container() const {
  static List<TimeSeries> dummyContainer;
  static bool initialized = false;
  if (!initialized) {
    TimeSeries dummySeries;
    dummyContainer.push_back(dummySeries);
    initialized = true;
  }
  return dummyContainer;
}

void Country::load(std::ifstream& is) {
  std::string line, temp_code, temp_name;
  std::streampos pos;
  std::stringstream ss;

  // Clear all TimeSeries and previous data before loading new data
  close();

  pos = is.tellg();
  std::getline(is, line, '\n');
  ss.str(line);
  std::getline(ss, m_name, ',');
  std::getline(ss, m_code, ',');
  is.seekg(pos);

  while (!is.eof()) {
    std::string token;

    pos = is.tellg();
    // Get the line of the csv and the name and country code
    std::getline(is, line, '\n');
    ss.clear();
    ss.str(line);
    std::getline(ss, temp_name, ',');
    std::getline(ss, temp_code, ',');

    if (temp_name != m_name) {
      break;
    }

    // Load the TimeSeries using a stringstream that it pointing at the
    // TimeSeries name
    m_series.push_back(TimeSeries(ss));
  }
  if (!is.eof())
    is.seekg(pos);
}

void Country::list() {
  // Iterate through the TimeSeries's and print names
  std::cout << m_name << " " << m_code << " ";
  for (TimeSeries& t : m_series)
    std::cout << t.get_series_name() << " ";
  std::cout << std::endl;
}

void Country::add(const std::string& series, const uint32_t year,
                  const uint32_t data) {
  // Iterate through TimeSeries's to find a matching code
  List<TimeSeries>::ListIterator it = find_series(series);
  if (it != m_series.end()) {
    it->add_data_point(year, data);
    return;
  }
  // No matching TimeSeries exists
  std::cout << "failure" << std::endl;
}

void Country::update(const std::string& series, const uint32_t year,
                     const uint32_t data) {
  // Iterate through TimeSeries's to find a matching code
  List<TimeSeries>::ListIterator it = find_series(series);
  if (it != m_series.end()) {
    it->update_data_point(year, data);
    return;
  }
  // No matching TimeSeries exists
  std::cout << "failure" << std::endl;
}

void Country::print(const std::string& series) {
  // Iterate through TimeSeries's to find a matching code
  List<TimeSeries>::ListIterator it = find_series(series);
  if (it != m_series.end()) {
    it->print();
    return;
  }
  // No matching TimeSeries exists
  std::cout << "failure" << std::endl;
}

void Country::delete_series(const std::string& series) {
  // Iterate through TimeSeries's to find a matching code
  List<TimeSeries>::ListIterator it = find_series(series);
  if (it != m_series.end()) {
    m_series.erase(find_series(series));
    std::cout << "success" << std::endl;
    return;
  }
  // No matching TimeSeries exists
  std::cout << "failure" << std::endl;
}

List<TimeSeries>::ListIterator Country::find_series(const std::string& series) {
  for (List<TimeSeries>::ListIterator it = m_series.begin();
       it != m_series.end(); ++it)
    if (it->get_series_code() == series)
      return it;
  return dummy_series_container().begin();
}

List<TimeSeries>::ListIterator
Country::find_series(const std::string& series) const {
  for (List<TimeSeries>::ListIterator it = m_series.begin();
       it != m_series.end(); ++it)
    if (it->get_series_code() == series)
      return it;
  return dummy_series_container().begin();
}

std::string Country::get_country_name() {
  return m_name;
}

List<TimeSeries>::ListIterator Country::end() const {
  return m_series.end();
}
