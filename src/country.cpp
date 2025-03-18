/*
  Implementation of the Country class.
  This file contains the function definitions for managing and performing
  calculations on TimeSeries lists, including largest mean calculations,
  deleting Time Series's, and loading TimeSeries for countries.
*/

#include "../include/country.hpp"
#include "../data_structures/include/file.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cfloat>

unsigned long long
Country::CountryHash::operator()(const std::string& seriesCode) const {
  unsigned long long W = 0, n = seriesCode.size();
  for (unsigned int i = n; i > 0; --i)
    W = W * 29 + seriesCode[n - i] - 'A';
  return W;
}

Country::Country() : m_series(m_hash) {}

Country::Country(const Country& other)
    : m_name(other.m_name), m_code(other.m_code), m_series(m_hash) {
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
  if (!m_series.contains(key))
    return dummy_series();
  return m_series[key];
}

TimeSeries Country::operator[](const std::string& key) const {
  return m_series.at(key);
}

void Country::close() {
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
  close();

  peek_line(is, line);
  ss.str(line);
  std::getline(ss, m_name, ',');
  std::getline(ss, m_code, ',');

  while (true) {
    peek_line(is, temp_name, ',');

    if (temp_name == "")
      return 0;

    if (temp_name != m_name)
      return 1;

    ss.clear();
    std::getline(is, line);
    ss.str(line);

    std::getline(ss, line, ',');
    std::getline(ss, line, ',');

    auto pos = ss.tellg();
    std::getline(ss, line, ',');
    std::getline(ss, line, ',');
    ss.seekg(pos);

    m_seriesCodes.push_back(line); // Store the series code in order
    m_series[line].load(ss);
  }
  return 1;
}

void Country::list() const {
  // Iterate through the TimeSeries's and print names
  std::cout << m_name << " " << m_code << " ";
  for (auto& t : m_seriesCodes)
    std::cout << m_series.at(t).get_series_name() << " ";
  std::cout << std::endl;
}

std::string Country::get_country_name() const {
  return m_name;
}

std::string Country::get_country_code() const {
  return m_code;
}

Map<std::string, TimeSeries, Country::CountryHash>::Iterator
Country::end() const {
  return m_series.end();
}
