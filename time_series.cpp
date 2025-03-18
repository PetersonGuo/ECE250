/*
  Implementation of the TimeSeries class.
  This file contains the function definitions for managing and performing
  calculations on time-series data, including monotonicity checks, mean
  calculation, and best-fit line determination.
*/
#include "time_series.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>


TimeSeries::TimeSeries(std::stringstream& ss)
    : m_bIsMonotonic(0), m_mean(0), m_slope(0), m_intercept(0), m_cache(0)
{
  load(ss);
}


TimeSeries::TimeSeries()
    : m_bIsMonotonic(0), m_mean(0), m_slope(0), m_intercept(0), m_cache(0)
{}

// Copy assignment operator
TimeSeries::TimeSeries(const TimeSeries& other)
    : m_data(other.m_data), m_bIsMonotonic(other.m_bIsMonotonic),
      m_mean(other.m_mean), m_slope(other.m_slope),
      m_intercept(other.m_intercept), m_cache(other.m_cache),
      m_seriesName(other.m_seriesName), m_seriesCode(other.m_seriesCode)
{}

TimeSeries::~TimeSeries()
{
  close();
}

TimeSeries& TimeSeries::operator=(const TimeSeries& other)
{
  if (this != &other) {
    m_data = other.m_data;
    m_bIsMonotonic = other.m_bIsMonotonic;
    m_mean = other.m_mean;
    m_slope = other.m_slope;
    m_intercept = other.m_intercept;
    m_cache = other.m_cache;
    m_seriesName = other.m_seriesName;
    m_seriesCode = other.m_seriesCode;
  }
  return *this;
}

TimeSeries::TimeSeries(TimeSeries&& other) noexcept
    : m_data(std::move(other.m_data)), m_bIsMonotonic(other.m_bIsMonotonic),
      m_mean(other.m_mean), m_slope(other.m_slope),
      m_intercept(other.m_intercept), m_cache(other.m_cache),
      m_seriesName(std::move(other.m_seriesName)),
      m_seriesCode(std::move(other.m_seriesCode))
{
  other.close();
}

TimeSeries& TimeSeries::operator=(TimeSeries&& other) noexcept
{
  if (this != &other) {
    m_data = std::move(other.m_data);
    m_bIsMonotonic = other.m_bIsMonotonic;
    m_mean = other.m_mean;
    m_slope = other.m_slope;
    m_intercept = other.m_intercept;
    m_cache = other.m_cache;
    m_seriesName = std::move(other.m_seriesName);
    m_seriesCode = std::move(other.m_seriesCode);
    other.close();
  }
  return *this;
}

bool TimeSeries::operator==(const TimeSeries& other) const
{
  return (m_seriesName == other.m_seriesName &&
          m_seriesCode == other.m_seriesCode);
}

bool TimeSeries::operator!=(const TimeSeries& other) const
{
  return (m_seriesName != other.m_seriesName ||
          m_seriesCode != other.m_seriesCode);
}

void TimeSeries::close()
{
  m_data.close();
  m_seriesCode.clear();
  m_seriesName.clear();
  m_bIsMonotonic = false;
  m_mean = 0.0;
  m_slope = 0.0;
  m_intercept = 0.0;
  m_cache = 0;
}

void TimeSeries::load(std::stringstream& ss)
{
  uint64_t i = 0;
  std::string token;

  close();

  std::getline(ss, m_seriesName, ',');
  std::getline(ss, m_seriesCode, ',');
  while (!ss.eof()) {
    double data;
    std::getline(ss, token, ',');
    data = std::stod(token);
    if (data != -1)
      m_data.emplace_back(i + 1960, data);
    ++i;
  }

  calc_mean();
}

bool TimeSeries::calc_is_monotonic()
{
  uint64_t i;
  bool decreasing = 1, increasing = 1;
  for (i = 1; i < m_data.size(); ++i) {
    if (!decreasing && !increasing)
      break;
    if (m_data[i].getSecond() < m_data[i - 1].getSecond())
      increasing = 0;
    else if (m_data[i].getSecond() > m_data[i - 1].getSecond())
      decreasing = 0;
  }
  return m_bIsMonotonic = (decreasing || increasing);
}

void TimeSeries::calc_best_fit()
{
  const long long N = m_data.size();
  uint64_t i;
  long long sx = 0, sy = 0, sxy = 0, sxx = 0;
  double bottom;
  m_slope = 0, m_intercept = 0;
  for (i = 0; i < (uint64_t)N; ++i) {
    sxy += m_data[i].getFirst() * m_data[i].getSecond();
    sx += m_data[i].getFirst();
    sy += m_data[i].getSecond();
    sxx += m_data[i].getFirst() * m_data[i].getFirst();
  }
  bottom = (N * sxx - sx * sx);
  if (N != 0 && bottom != 0) {
    m_slope = (N * sxy - sx * sy) / bottom;
    m_intercept = (sy - m_slope * sx) / (double)N;
  }
}

double TimeSeries::calc_mean()
{
  uint64_t i;
  double sum = 0;
  for (i = 0; i < m_data.size(); ++i)
    sum += m_data[i].getSecond();
  if (m_data.size())
    m_mean = sum / m_data.size();
  return m_mean;
}

bool TimeSeries::is_monotonic()
{
  if (!GET_BIT(m_cache, MONOTONIC_BIT)) {
    calc_is_monotonic();
    SET_BIT(m_cache, MONOTONIC_BIT, 1);
  }

  if (m_data.size()) {
    std::cout << (m_bIsMonotonic ? "series is monotonic"
                                 : "series is not monotonic")
              << std::endl;
  }
  else {
    std::cout << "failure" << std::endl;
  }
  return m_bIsMonotonic;
}

void TimeSeries::best_fit(double& m, double& b)
{
  if (!GET_BIT(m_cache, FIT_BIT)) {
    calc_best_fit();
    SET_BIT(m_cache, FIT_BIT, 1);
  }

  if (m_data.size()) {
    printf("slope is %lf intercept is %lf\n", m_slope, m_intercept);
  }
  else {
    std::cout << "failure" << std::endl;
  }

  m = m_slope;
  b = m_intercept;
}

double TimeSeries::mean()
{
  if (!GET_BIT(m_cache, MEAN_BIT)) {
    calc_mean();
    SET_BIT(m_cache, MEAN_BIT, 1);
  }

  return m_mean;
}

bool TimeSeries::add_data_point(const int time, const int value)
{
  int l = 0, r = m_data.size() - 1;
  while (l <= r) {
    int m = (l + r) / 2;
    if (m_data[m].getFirst() < time) {
      l = m + 1;
    }
    else if (m_data[m].getFirst() > time) {
      r = m - 1;
    }
    else {
      std::cout << "failure" << std::endl;
      return false;
    }
  }

  m_data.emplace(l, time, value);
  reset_cache();
  std::cout << "success" << std::endl;
  return true;
}

bool TimeSeries::update_data_point(int time, int value)
{
  long long n = m_data.size();
  long long l = 0, r = n - 1;
  while (l <= r) {
    uint64_t m = (l + r) / 2;
    if (m_data[m].getFirst() < time) {
      l = m + 1;
    }
    else if (m_data[m].getFirst() > time) {
      r = m - 1;
    }
    else {
      l = m;
      break;
    }
  }

  if (l < n && m_data[l].getFirst() == time) {
    m_data[l].getSecond() = value;
    reset_cache();
    std::cout << "success" << std::endl;
    return true;
  }
  std::cout << "failure" << std::endl;
  return false;
}

void TimeSeries::print() const
{
  if (!m_data.size()) {
    std::cout << "failure" << std::endl;
    return;
  }

  for (uint64_t i = 0; i < m_data.size(); ++i)
    std::cout << '(' << m_data[i].getFirst() << "," << m_data[i].getSecond()
              << ')' << " ";
  std::cout << std::endl;
}

void TimeSeries::reset_cache()
{
  m_cache = 0;
}

const std::string TimeSeries::get_series_name() const
{
  return m_seriesName;
}

const std::string TimeSeries::get_series_code() const
{
  return m_seriesCode;
}

const size_t TimeSeries::get_size() const
{
  return m_data.size();
}
