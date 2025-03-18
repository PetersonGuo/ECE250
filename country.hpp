/*
  Country class header file.
  This file defines the Country class, which manages multiple time-series
  datasets for a specific country. It provides methods to load data, add,
  update, print, and delete time-series records, as well as determine the
  largest dataset.
*/

#ifndef COUNTRY
#define COUNTRY

#include "time_series.hpp"
#include "data_structures/list.hpp"

/*
  Country class represents a collection of time-series data for a specific
  country. It manages multiple TimeSeries objects and provides functionality to
  add, update, list, and delete data points, as well as compute the largest
  dataset.
*/

class Country
{
private:
  std::string m_name, m_code,
      m_biggest;             // Country name, code, and largest dataset
  bool m_cache;              // Cache flag to avoid redundant calculations
  List<TimeSeries> m_series; // Stores all time-series data for the country

public:
  /**
   * @brief Default constructor that initializes an empty Country object.
   */
  Country();

  /**
   * @brief Loads time-series data for the given country.
   * @param country The name of the country whose data should be loaded.
   */
  void load(const std::string&);

  /**
   * @brief Clears all stored data and resets the Country object.
   */
  void close();

  /**
   * @brief Lists all available time-series datasets for the country.
   * Prints the country name, code, and all stored series.
   */
  void list();

  /**
   * @brief Adds a new data point to a specified time series.
   * @param series The series code identifying the time series.
   * @param year The year of the data point.
   * @param data The value associated with the given year.
   */
  void add(const std::string&, const uint32_t, const uint32_t);

  /**
   * @brief Updates an existing data point in a specified time series.
   * @param series The series code identifying the time series.
   * @param year The year of the data point to update.
   * @param data The new value to be assigned to the given year.
   */
  void update(const std::string&, const uint32_t, const uint32_t);

  /**
   * @brief Prints all data points for the given time series.
   * @param series The series code identifying the time series to print.
   */
  void print(const std::string&);

  /**
   * @brief Deletes a specified time series from the country's dataset.
   * @param series The series code identifying the time series to delete.
   */
  void delete_series(const std::string&);

  /**
   * @brief Computes which time series has the highest mean value.
   * The result is stored and cached in `m_biggest` for quick retrieval.
   */
  void calc_biggest();

  /**
   * @brief Prints the name of the time series with the highest mean.
   * If not already cached, it will compute the result first.
   */
  void biggest();
};

#endif
