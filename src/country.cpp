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

Country::Country() : m_hash(this), m_series(m_hash) {}

Country::Country(const Country& other)
    : m_name(other.m_name), m_code(other.m_code), m_hash(this),
      m_series(m_hash) {
  m_series = other.m_series;
}

Country& Country::operator=(const Country& other) {
  if (this != &other) {
    m_name = other.m_name;
    m_code = other.m_code;
    m_series = other.m_series;
    m_hash = CountryHash(this);
  }
  return *this;
}

Country::Country(Country&& other) noexcept
    : m_name(std::move(other.m_name)), m_code(std::move(other.m_code)),
      m_hash(this), m_series(std::move(other.m_series)) {}

Country& Country::operator=(Country&& other) noexcept {
  if (this != &other) {
    m_name = std::move(other.m_name);
    m_code = std::move(other.m_code);
    m_series = std::move(other.m_series);
  }
  return *this;
}

TimeSeries& Country::operator[](const std::string& key) {
  if (!m_series.contains(key) || m_series[key].get_series_code() != key)
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
  return dummySeries;
}

void Country::load(std::ifstream& is) {
  std::string line, temp_name;
  std::stringstream ss;

  // Clear all TimeSeries and previous data before loading new data
  close();

  peek_line(is, line);
  ss.str(line);
  std::getline(ss, m_name, ',');
  std::getline(ss, m_code, ',');

  while (!is.eof()) {
    std::string token;

    ss.clear();

    peek_line(is, line);
    ss.str(line);
    std::getline(ss, temp_name, ',');
    std::getline(ss, line, ',');

    if (temp_name != m_name)
      break;

    auto pos = ss.tellg();
    std::getline(ss, line, ',');
    std::getline(ss, line, ',');
    ss.seekg(pos);

    m_series[line] = TimeSeries(ss);
    std::getline(is, line, '\n');
  }
}

void Country::list() const {
  // Iterate through the TimeSeries's and print names
  std::cout << m_name << " " << m_code << " ";
  for (auto t : m_series)
    std::cout << t.getSecond().get_series_name() << " ";
  std::cout << std::endl;
}

void Country::add(const std::string& series, const uint32_t year,
                  const uint32_t data) {
  auto it = m_series.find(series);
  if (it != m_series.end()) {
    (*it).getSecond().add_data_point(year, data);
    return;
  }
  // No matching TimeSeries exists
  std::cout << "failure" << std::endl;
}

void Country::update(const std::string& series, const uint32_t year,
                     const uint32_t data) {
  auto it = m_series.find(series);
  if (it != m_series.end()) {
    (*it).getSecond().update_data_point(year, data);
    return;
  }
  // No matching TimeSeries exists
  std::cout << "failure" << std::endl;
}

void Country::print(const std::string& series) const {
  // Iterate through TimeSeries's to find a matching code
  auto it = m_series.find(series);
  if (it != m_series.end()) {
    (*it).getSecond().print();
    return;
  }
  // No matching TimeSeries exists
  std::cout << "failure" << std::endl;
}

void Country::delete_series(const std::string& series) {
  // Iterate through TimeSeries's to find a matching code
  auto it = m_series.find(series);
  if (it != m_series.end()) {
    m_series.erase(it);
    std::cout << "success" << std::endl;
    return;
  }
  // No matching TimeSeries exists
  std::cout << "failure" << std::endl;
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

int Country::CountryHash::operator()(const std::string& seriesCode) const {
  unsigned long W = 0, n = seriesCode.size();
  for (unsigned int i = n; i > 0; --i)
    W = W * 26 + seriesCode[n - i] - 'A';
  return W;
}
