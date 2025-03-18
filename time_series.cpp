/*
  Implementation of the TimeSeries class.
  This file contains the function definitions for managing and performing
  calculations on time-series data, including monotonicity checks, mean
  calculation, and best-fit line determination.
*/

#include "time_series.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/*
  Constructs a TimeSeries object by loading data from the specified file.
  Initializes member variables and caches.
  @param filename Name of the file to load the time-series data from.
*/
TimeSeries::TimeSeries(const std::string &filename)
    : m_bIsMonotonic(0), m_mean(0), m_slope(0), m_intercept(0), m_cache(0) {
  load_data(filename);
}

/*
  Default constructor that initializes an empty TimeSeries object.
  Sets all cached values and members to default.
*/
TimeSeries::TimeSeries()
    : m_bIsMonotonic(0), m_mean(0), m_slope(0), m_intercept(0), m_cache(0) {}

/*
  Clears all data in the TimeSeries object and resets the internal state.
*/
void TimeSeries::close() { m_data.clear(); }

/*
  Loads time-series data from a file and populates the internal data structure.
  Performs initial calculations for monotonicity, mean, and best-fit line.
  @param filename Name of the file to load data from.
  @return Status message indicating success or failure.
*/
std::string TimeSeries::load_data(const std::string &filename) {
  close();
  std::string line;
  std::ifstream is;

  is.open(filename);
  if (is.fail()) {
    throw std::invalid_argument("invalid file");
  }

  uint64_t i = 0;
  while (!is.eof()) {
    std::stringstream ss;
    std::string token;
    std::getline(is, line, '\n');
    ss.str(line);
    std::getline(ss, token, ',');
    std::getline(ss, token, ',');
    std::getline(ss, m_seriesName, ',');
    std::getline(ss, m_seriesCode, ',');
    while (!ss.eof()) {
      int data;
      std::getline(ss, token, ',');
      data = std::stoi(token);
      if (data != -1)
        m_data.push_back(Pair<int, int>(i + 1960, data));
      ++i;
    }
  }
  std::cout << "success" << std::endl;
  is.close();

  calc_is_monotonic();
  calc_best_fit();
  calc_mean();

  return "";
}

/*
  Determines whether the time-series data is monotonic (non-increasing or
  non-decreasing). Updates the cached monotonicity value.
  @return True if the data is monotonic, false otherwise.
*/
bool TimeSeries::calc_is_monotonic() {
  uint64_t i;
  bool decreasing = 0, monotonic = 1, can_change = 1;
  for (i = 1; i < m_data.size(); ++i) {
    if (can_change) {
      if (m_data[i - 1].getSecond() != m_data[i].getSecond()) {
        decreasing = m_data[i - 1].getSecond() > m_data[i].getSecond();
        can_change = 0;
      }
    } else {
      if (m_data[i - 1].getSecond() > m_data[i].getSecond() && !decreasing) {
        monotonic = false;
        break;
      } else if (m_data[i - 1].getSecond() < m_data[i].getSecond() &&
                 decreasing) {
        monotonic = false;
        break;
      }
    }
  }
  m_bIsMonotonic = monotonic;
  return monotonic;
}

/*
  Calculates the slope and intercept of the best-fit line using linear
  regression. Updates the cached slope and intercept values.
*/
void TimeSeries::calc_best_fit() {
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

/*
  Computes the mean value of the time-series data.
  Updates the cached mean value.
  @return The calculated mean value.
*/
double TimeSeries::calc_mean() {
  uint64_t i;
  double sum = 0;
  for (i = 0; i < m_data.size(); ++i)
    sum += m_data[i].getSecond();
  if (m_data.size())
    m_mean = sum / m_data.size();
  return m_mean;
}

/*
  Checks if the series is monotonic and prints the result.
  If not already cached, calculates and caches the result.
  @return True if the series is monotonic, false otherwise.
*/
bool TimeSeries::is_monotonic() {
  if (!GET_BIT(m_cache, MONOTONIC_BIT)) {
    calc_is_monotonic();
    SET_BIT(m_cache, MONOTONIC_BIT, 1);
  }

  if (m_data.size()) {
    std::cout << (m_bIsMonotonic ? "series is monotonic"
                                 : "series is not monotonic")
              << std::endl;
  } else {
    std::cout << "failure" << std::endl;
  }
  return m_bIsMonotonic;
}

/*
  Calculates the best-fit line parameters and prints them.
  If not already cached, calculates and caches the result.
  @param m Reference to store the slope.
  @param b Reference to store the intercept.
*/
void TimeSeries::best_fit(double &m, double &b) {
  if (!GET_BIT(m_cache, FIT_BIT)) {
    calc_best_fit();
    SET_BIT(m_cache, FIT_BIT, 1);
  }

  if (m_data.size()) {
    printf("slope is %lf intercept is %lf\n", m_slope, m_intercept);
  } else {
    std::cout << "failure" << std::endl;
  }

  m = m_slope;
  b = m_intercept;
}

/*
  Computes and prints the mean value of the series.
  If not already cached, calculates and caches the result.
  @return The mean value.
*/
double TimeSeries::mean() {
  if (!GET_BIT(m_cache, MEAN_BIT)) {
    calc_mean();
    SET_BIT(m_cache, MEAN_BIT, 1);
  }

  std::cout << (m_data.size() ? "mean is " + std::to_string(m_mean) : "failure")
            << std::endl;
  return m_mean;
}

/*
  Adds a new data point to the time series.
  Ensures that the data point is unique and maintains sorted order.
  @param time The year of the data point.
  @param value The value associated with the year.
  @return True if the data point was successfully added, false otherwise.
*/
bool TimeSeries::add_data_point(const int time, const int value) {
  int l = 0, r = m_data.size() - 1;
  while (l <= r) {
    int m = (l + r) / 2;
    if (m_data[m].getFirst() < time) {
      l = m + 1;
    } else if (m_data[m].getFirst() > time) {
      r = m - 1;
    } else {
      std::cout << "failure" << std::endl;
      return false;
    }
  }

  m_data.insert(l, Pair<int, int>(time, value));
  reset_cache();
  std::cout << "success" << std::endl;
  return true;
}

/*
  Updates the value of an existing data point.
  Finds the data point by year and updates its value if it exists.
  @param time The year of the data point to update.
  @param value The new value to assign to the year.
  @return True if the data point was successfully updated, false otherwise.
*/
bool TimeSeries::update_data_point(int time, int value) {
  long long l = 0, r = (long long)m_data.size() - 1;
  while (l <= r) {
    int m = (l + r) / 2;
    if (m_data[m].getFirst() < time) {
      l = m + 1;
    } else if (m_data[m].getFirst() > time) {
      r = m - 1;
    } else {
      l = m;
      break;
    }
  }

  if (l < m_data.size() && m_data[l].getFirst() == time) {
    m_data[l].getSecond() = value;
    reset_cache();
    std::cout << "success" << std::endl;
    return true;
  }
  std::cout << "failure" << std::endl;
  return false;
}

/*
  Prints all the data points in the time series.
  Outputs each data point as a pair of year and value.
*/
void TimeSeries::print() {
  if (!m_data.size()) {
    std::cout << "failure" << std::endl;
    return;
  }

  for (uint64_t i = 0; i < m_data.size(); ++i)
    std::cout << '(' << m_data[i].getFirst() << "," << m_data[i].getSecond()
              << ')' << " ";
  std::cout << std::endl;
}

/*
  Resets the cached computed properties (monotonicity, mean, best-fit).
  Invalidates all cached values, forcing recalculation when needed.
*/
void TimeSeries::reset_cache() { m_cache = 0; }
