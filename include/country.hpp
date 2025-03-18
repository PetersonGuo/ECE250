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
#include "../data_structures/include/vector.hpp"
#include "../data_structures/include/unordered_map.hpp"

/*
  Country class represents a collection of time-series data for a specific
  country. It manages multiple TimeSeries objects and provides functionality to
  add, update, list, and delete data points, as well as compute the largest
  dataset.
*/
class Country {
public:
  /**
   * @brief Hash function object for computing the hash value of a series code.
   */
  class CountryHash {
  public:
    /**
     * @brief Hash function to compute the hash value of a series code.
     * @param seriesCode The series code to hash.
     * @return The computed hash value.
     */
    unsigned long long operator()(const std::string&) const;
  };

private:
  std::string m_name, m_code; // Country name and code
  static CountryHash m_hash;
  Vector<std::string> m_seriesCodes; // List of series codes
  Map<std::string, TimeSeries, CountryHash>
      m_series; // Stores all time-series data for the country

public:
  /**
   * @brief Constructs an empty Country object.
   */
  Country();

  /**
   * @brief Copy constructor.
   * @param other The Country object to copy.
   */
  Country(const Country& other);

  /**
   * @brief Copy assignment operator.
   * @param other The Country object to copy.
   * @return Reference to the assigned object.
   */
  Country& operator=(const Country& other);

  /**
   * @brief Move constructor.
   * @param other The Country object to move.
   */
  Country(Country&& other) noexcept;

  /**
   * @brief Move assignment operator.
   * @param other The Country object to move.
   * @return Reference to the assigned object.
   */
  Country& operator=(Country&& other) noexcept;

  /**
   * @brief Finds a specific time series in the list (const version).
   * @param series The series code to find.
   * @return Const iterator to the found time series.
   */
  TimeSeries& operator[](const std::string&);

  /**
   * @brief Finds a specific time series in the list.
   * @param series The series code to find.
   * @return Const iterator to the found time series.
   */
  TimeSeries operator[](const std::string&) const;

  /**
   * @brief Returns a dummy container with a mean of 0.
   * @return Const reference to a dummy container of time-series data.
   */
  TimeSeries& dummy_series() const;

  /**
   * @brief Loads time-series data for the given country.
   * @param is Input filestream containing the country's data.
   * @return True if at EOF, false otherwise.
   */
  bool load(std::ifstream& is);

  /**
   * @brief Clears all stored data and resets the Country object.
   */
  void close();

  /**
   * @brief Lists all available time-series datasets for the country.
   * Prints the country name, code, and all stored series.
   */
  void list() const;

  /**
   * @brief Retrieves the country's name.
   * @return The country name as a string.
   */
  std::string get_country_name() const;

  /**
   * @brief Retrieves the country's code.
   * @return The country code as a string.
   */
  std::string get_country_code() const;

  /**
   * @brief Returns an iterator to the end of the list.
   * @return Iterator pointing to the end of the list.
   */
  Map<std::string, TimeSeries, CountryHash>::Iterator end() const;
};

#endif
