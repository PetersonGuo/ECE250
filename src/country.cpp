/*
  Implementation of the Country class.
  This file contains the function definitions for managing and performing
  calculations on TimeSeries lists, including largest mean calculations,
  deleting Time Series's, and loading TimeSeries for countries.
*/

#include "../include/country.hpp"
#include "../include/file.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cfloat>

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

TimeSeries& Country::operator[](const std::string& key) {
  if (!m_series.count(key))
    return dummy_series();
  return m_series[key];
}

TimeSeries Country::operator[](const std::string& key) const {
  return m_series.at(key);
}

void Country::clear() {
  m_name = "";
  m_code = "";
  m_series.clear();
}

TimeSeries& Country::dummy_series() const {
  static TimeSeries dummySeries;
  dummySeries.mean();
  return dummySeries;
}

bool Country::load(std::ifstream& is) {
  std::string line, temp_name;
  std::stringstream ss;

  // Clear all TimeSeries and previous data before loading new data
  clear();

  peekline(is, line);
  ss.str(line);
  std::getline(ss, m_name, ',');
  std::getline(ss, m_code, ',');

  while (true) {
    peekline(is, temp_name, ',');

    if (temp_name == "")
      return 0;

    if (temp_name != m_name)
      return 1;

    ss.clear();
    std::getline(is, line);
    ss.str(line);

    std::getline(ss, line, ',');
    std::getline(ss, line, ',');

    std::fpos<std::mbstate_t> pos = ss.tellg();
    std::getline(ss, line, ',');
    std::getline(ss, line, ',');
    ss.seekg(pos);

    m_series[line].load(ss);
  }
  return 1;
}

std::string Country::get_country_name() const {
  return m_name;
}

std::string Country::get_country_code() const {
  return m_code;
}
