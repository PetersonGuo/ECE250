/*
  TimeSeries class header file.
  This file defines the TimeSeries class, which is used to store and manipulate
  time-series data. It includes methods to calculate properties such as
  monotonicity, mean, and best-fit line, along with functions to add, update,
  and print data points.
*/

// Inline functions to get/set bit values
#define SET_BIT(value, shift, set)                                             \
  (value = (value & ~(1 << shift)) | (set << shift))
#define GET_BIT(value, shift) ((value >> shift) & 1)

// Bit indexes of cache bits used to represent m_cache
#define MONOTONIC_BIT 0
#define MEAN_BIT 1
#define FIT_BIT 2

#ifndef TIMESERIES
#define TIMESERIES

#include "data_structures/pair.hpp"
#include "data_structures/vector.hpp"
#include <string>

class TimeSeries {
private:
  Vector<Pair<int, int>> m_data; // Stores the time series data as pairs where
                                 // first value is year and second is data

  bool m_bIsMonotonic;         // Cached value of calc_monotonic function
  double m_mean;               // Cached value of calc_mean function
  double m_slope, m_intercept; // Cached values of calc_fit function

  unsigned char m_cache; // Valid cache bool bits where <00000000> is a bitset
                         // and from right to left, bits represent monotonic
                         // cache valid, mean cache valid, and fit cache valid

  std::string m_seriesName, m_seriesCode; // TimeSeries name and code

public:
  /*
    Constructs a TimeSeries object by loading data from a file.
    @param filename Name of the file to load the time-series data from.
  */
  TimeSeries(const std::string &);

  /*
    Default constructor that initializes an empty TimeSeries object.
  */
  TimeSeries();

  /*
    Clears all data and resets the TimeSeries object to its initial state.
  */
  void close();

  /*
    Loads time-series data from a specified file.
    Parses the file and populates the internal data structure.
    @param filename Name of the file to load data from.
    @return Status message indicating success or failure.
  */
  std::string load_data(const std::string &);

  /*
    Calculates whether the data is monotonic (non-increasing or non-decreasing).
    Iterates through the data points to determine if the series is monotonic.
    @return True if the data is monotonic, false otherwise.
  */
  bool calc_is_monotonic();

  /*
    Computes the slope and intercept of the best-fit line using linear
    regression. The function calculates these values based on the current data.
  */
  void calc_best_fit();

  /*
    Calculates the mean of the time-series data.
    Computes the average of all data points in the series.
    @return The mean value.
  */
  double calc_mean();

  /*
    Checks if the data is monotonic. If not already cached, calculates and
    caches the result. Uses the cached monotonicity value if valid.
    @return True if the data is monotonic, false otherwise.
  */
  bool is_monotonic();

  /*
    Calculates the best-fit line parameters. If not already cached, calculates
    and caches the result. Uses the cached slope and intercept values if valid.
    @param slope Reference to store the slope of the best-fit line.
    @param intercept Reference to store the intercept of the best-fit line.
  */
  void best_fit(double &, double &);

  /*
    Returns the mean of the data. If not already cached, calculates and caches
    the result. Uses the cached mean value if valid.
    @return The mean value.
  */
  double mean();

  /*
    Adds a new data point to the time series.
    Ensures that the data point is unique and maintains sorted order.
    @param year The year of the data point.
    @param value The value associated with the year.
    @return True if the data point was successfully added, false otherwise.
  */
  bool add_data_point(int, int);

  /*
    Prints all the data points in the time series.
    Outputs each data point as a pair of year and value.
  */
  void print();

  /*
    Updates the value of an existing data point.
    Finds the data point by year and updates its value if it exists.
    @param year The year of the data point to update.
    @param value The new value to assign to the year.
    @return True if the data point was successfully updated, false otherwise.
  */
  bool update_data_point(int, int);

  /*
    Resets the cached computed properties (monotonicity, mean, best-fit).
    Invalidates all cached values, forcing recalculation when needed.
  */
  void reset_cache();
};
#endif
