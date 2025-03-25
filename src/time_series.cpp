/*
  Implementation of the TimeSeries class.
  This file contains the function definitions for managing and performing
  calculations on time-series data, including monotonicity checks, mean
  calculation, and best-fit line determination.
*/

#include "../include/time_series.hpp"

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>

TimeSeries::TimeSeries(std::stringstream& ss)
    : m_mean(0), m_cache(0), m_invalid(1) {
  load(ss);
}

TimeSeries::TimeSeries() : m_mean(0), m_cache(0), m_invalid(1) {}

// Copy assignment operator
TimeSeries::TimeSeries(const TimeSeries& other)
    : m_data(other.m_data), m_mean(other.m_mean), m_cache(other.m_cache),
      m_invalid(other.m_invalid), m_seriesName(other.m_seriesName),
      m_seriesCode(other.m_seriesCode) {}

TimeSeries::~TimeSeries() {
  clear();
}

TimeSeries& TimeSeries::operator=(const TimeSeries& other) {
  if (this != &other) {
    m_data = other.m_data;
    m_mean = other.m_mean;
    m_cache = other.m_cache;
    m_invalid = other.m_invalid;
    m_seriesName = other.m_seriesName;
    m_seriesCode = other.m_seriesCode;
  }
  return *this;
}

TimeSeries::TimeSeries(TimeSeries&& other) noexcept
    : m_data(std::move(other.m_data)), m_mean(other.m_mean),
      m_cache(other.m_cache), m_invalid(other.m_invalid),
      m_seriesName(std::move(other.m_seriesName)),
      m_seriesCode(std::move(other.m_seriesCode)) {
  other.clear();
}

TimeSeries& TimeSeries::operator=(TimeSeries&& other) noexcept {
  if (this != &other) {
    m_data = std::move(other.m_data);
    m_mean = other.m_mean;
    m_cache = other.m_cache;
    m_invalid = other.m_invalid;
    m_seriesName = std::move(other.m_seriesName);
    m_seriesCode = std::move(other.m_seriesCode);
    other.clear();
  }
  return *this;
}

void TimeSeries::clear() {
  m_data.clear();
  m_seriesCode.clear();
  m_seriesName.clear();
  m_mean = 0.0;
  m_cache = 0;
}

void TimeSeries::load(std::stringstream& ss) {
  uint64_t i = 0;
  std::string token;

  clear();
  m_data.reserve(2024 - 1960);

  std::getline(ss, m_seriesName, ',');
  std::getline(ss, m_seriesCode, ',');
  while (std::getline(ss, token, ',')) {
    std::string_view token_view(token);
    if (token_view.compare(0, 2, "-1") != 0) {
      m_data.emplace_back(i + 1960, std::stod(token));
      m_invalid = 0;
    }
    ++i;
  }

  calc_mean();
}

double TimeSeries::calc_mean() {
  uint64_t i;
  double sum = 0;
  for (i = 0; i < m_data.size(); ++i)
    sum += m_data[i].second;
  if (m_data.size())
    m_mean = sum / m_data.size();
  return m_mean;
}

double TimeSeries::mean() {
  if (!m_cache) {
    calc_mean();
    m_cache = 1;
  }

  return m_mean;
}

const std::string& TimeSeries::get_series_name() const {
  return m_seriesName;
}

const std::string& TimeSeries::get_series_code() const {
  return m_seriesCode;
}

bool TimeSeries::invalid() const {
  return m_invalid;
}
