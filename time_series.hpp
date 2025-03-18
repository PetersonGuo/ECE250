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
#include <sstream>
#include <string>

/*
  TimeSeries class represents a dataset containing time-series data points,
  where each data point consists of a year and a corresponding value. It
  provides functionality to add, update, print, and delete data points, as well
  as perform statistical operations such as calculating the mean, checking
  monotonicity, and determining the best-fit line.
*/

class TimeSeries
{
private:
  Vector<Pair<int, double>>
      m_data;           // Stores the time-series data as (year, value) pairs

  bool m_bIsMonotonic;  // Cached value indicating whether the data is monotonic
  double m_mean;        // Cached mean value of the dataset
  double m_slope,
      m_intercept;      // Cached slope and intercept for the best-fit line

  unsigned char m_cache; // Bitset used for caching calculations
                         // Bits represent: [Monotonic, Mean, Fit]

  std::string m_seriesName,
      m_seriesCode; // Stores the name and code of the time series

public:
  /**
   * @brief Default constructor that initializes an empty TimeSeries object.
   */
  TimeSeries();

  /**
   * @brief Constructs a TimeSeries object by loading data from a stream.
   * @param ss The input stream containing time-series data.
   */
  TimeSeries(std::stringstream&);

  /**
   * @brief Destructor for TimeSeries.
   * Clears all stored data.
   */
  ~TimeSeries();

  /**
   * @brief Copy constructor for TimeSeries.
   * @param other The TimeSeries object to copy from.
   */
  TimeSeries(const TimeSeries&);

  /**
   * @brief Copy assignment operator.
   * @param other The TimeSeries object to assign from.
   * @return A reference to this TimeSeries object.
   */
  TimeSeries& operator=(const TimeSeries&);

  /**
   * @brief Move constructor for TimeSeries.
   * Transfers ownership of resources from another object.
   * @param other The TimeSeries object to move from.
   */
  TimeSeries(TimeSeries&&) noexcept;

  /**
   * @brief Move assignment operator.
   * Transfers ownership of resources from another object.
   * @param other The TimeSeries object to move from.
   * @return A reference to this TimeSeries object.
   */
  TimeSeries& operator=(TimeSeries&&) noexcept;

  /**
   * @brief Equality operator to compare two TimeSeries objects.
   * @param other The TimeSeries object to compare with.
   * @return True if both objects represent the same series, false otherwise.
   */
  bool operator==(const TimeSeries&) const;

  /**
   * @brief Inequality operator to compare two TimeSeries objects.
   * @param other The TimeSeries object to compare with.
   * @return True if the objects represent different series, false otherwise.
   */
  bool operator!=(const TimeSeries&) const;

  /**
   * @brief Clears all data and resets the TimeSeries object to its initial
   * state.
   */
  void close();

  /**
   * @brief Loads time-series data from an input stream.
   * Parses the stream and populates the internal data structure.
   * @param ss The input stream containing time-series data.
   */
  void load(std::stringstream&);

  /**
   * @brief Checks whether the time-series data is monotonic.
   * @return True if the data is monotonic (non-increasing or non-decreasing),
   * false otherwise.
   */
  bool calc_is_monotonic();

  /**
   * @brief Computes the slope and intercept of the best-fit line using linear
   * regression.
   */
  void calc_best_fit();

  /**
   * @brief Calculates the mean of the time-series data.
   * @return The computed mean value.
   */
  double calc_mean();

  /**
   * @brief Determines whether the data is monotonic.
   * If not already cached, calculates and caches the result.
   * @return True if the data is monotonic, false otherwise.
   */
  bool is_monotonic();

  /**
   * @brief Computes the best-fit line parameters.
   * If not already cached, calculates and caches the result.
   * @param slope Reference to store the computed slope.
   * @param intercept Reference to store the computed intercept.
   */
  void best_fit(double&, double&);

  /**
   * @brief Returns the mean of the time-series data.
   * If not already cached, calculates and caches the result.
   * @return The mean value of the data.
   */
  double mean();

  /**
   * @brief Adds a new data point to the time series.
   * Ensures that the data point is unique and maintains sorted order.
   * @param year The year of the data point.
   * @param value The associated value for that year.
   * @return True if the data point was successfully added, false otherwise.
   */
  bool add_data_point(int, int);

  /**
   * @brief Prints all data points in the time series.
   * Outputs each data point as a (year, value) pair.
   */
  void print() const;

  /**
   * @brief Updates the value of an existing data point.
   * Finds the data point by year and updates its value if it exists.
   * @param year The year of the data point to update.
   * @param value The new value to assign to that year.
   * @return True if the data point was successfully updated, false otherwise.
   */
  bool update_data_point(int, int);

  /**
   * @brief Resets the cached computed properties (monotonicity, mean,
   * best-fit). Invalidates all cached values, forcing recalculation when
   * needed.
   */
  void reset_cache();

  /**
   * @brief Gets the name of the time series.
   * @return The name of the series as a string.
   */
  const std::string get_series_name() const;

  /**
   * @brief Gets the code of the time series.
   * @return The code of the series as a string.
   */
  const std::string get_series_code() const;

  /**
   * @brief Gets the number of data points in the time series.
   * @return The size of the dataset.
   */
  const size_t get_size() const;
};

#endif // TIMESERIES
