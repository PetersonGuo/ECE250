/*
  TimeSeries class header file.
  This file defines the TimeSeries class, which is used to store and manipulate
  time-series data. It includes methods to calculate properties such as
  monotonicity, mean, and best-fit line, along with functions to add, update,
  and print data points.
*/

#ifndef TIMESERIES
#define TIMESERIES

#include "../data_structures/include/pair.hpp"
#include "../data_structures/include/vector.hpp"
#include <sstream>
#include <string>

/*
  TimeSeries class represents a dataset containing time-series data points,
  where each data point consists of a year and a corresponding value. It
  provides functionality to add, update, print, and delete data points, as well
  as perform statistical operations such as calculating the mean, checking
  monotonicity, and determining the best-fit line.
*/

class TimeSeries {
private:
  Vector<Pair<int, double>>
      m_data; // Stores the time-series data as (year, value) pairs

  double m_mean;  // Cached mean value of the dataset
  bool m_cache;   // Bitset used to check if cache is valid
  bool m_invalid; // Flag for an invalid TimeSeries

  std::string m_seriesName,
      m_seriesCode; // Stores the name and code of the time series

  /**
   * @brief Calculates the mean of the time-series data.
   * @return The computed mean value.
   */
  double calc_mean();

  /**
   * @brief Resets the cached computed properties (monotonicity, mean,
   * best-fit). Invalidates all cached values, forcing recalculation when
   * needed.
   */
  void reset_cache();

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
   * @brief Returns the mean of the time-series data.
   * If not already cached, calculates and caches the result.
   * @return The mean value of the data.
   */
  double mean();

  /**
   * @brief Gets the name of the time series.
   * @return The name of the series as a string.
   */
  const std::string& get_series_name() const;

  /**
   * @brief Gets the code of the time series.
   * @return The code of the series as a string.
   */
  const std::string& get_series_code() const;

  /**
   * @brief Gets the number of data points in the time series.
   * @return The size of the dataset.
   */
  size_t get_size() const;

  /**
   * @brief Returns a flag indicating if the TimeSeries contains valid data.
   * @return The validity of the TimeSeries data.
   */
  bool invalid() const;
};

#endif // TIMESERIES
