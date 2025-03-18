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
#include "../data_structures/include/unordered_map.hpp"

/*
  Country class represents a collection of time-series data for a specific
  country. It manages multiple TimeSeries objects and provides functionality to
  add, update, list, and delete data points, as well as compute the largest
  dataset.
*/
class Country {
public:
  class CountryHash {
  private:
    Country* m_data;

  public:
    CountryHash(Country* data) : m_data(data) {}
    CountryHash(const CountryHash& other) : m_data(other.m_data) {}
    CountryHash(CountryHash&& other) noexcept : m_data(other.m_data) {}
    CountryHash& operator=(const CountryHash& other) {
      if (this != &other) {
        m_data = other.m_data;
      }
      return *this;
    }
    int operator()(const std::string&) const;
  };

private:
  std::string m_name, m_code; // Country name and code
  CountryHash m_hash;
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
   */
  void load(std::ifstream& is);

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
   * @brief Adds a new data point to a specified time series.
   * @param series The series code identifying the time series.
   * @param year The year of the data point.
   * @param data The value associated with the given year.
   */
  void add(const std::string& series, const uint32_t year, const uint32_t data);

  /**
   * @brief Updates an existing data point in a specified time series.
   * @param series The series code identifying the time series.
   * @param year The year of the data point to update.
   * @param data The new value to be assigned to the given year.
   */
  void update(const std::string& series, const uint32_t year,
              const uint32_t data);

  /**
   * @brief Prints all data points for the given time series.
   * @param series The series code identifying the time series to print.
   */
  void print(const std::string& series) const;

  /**
   * @brief Deletes a specified time series from the country's dataset.
   * @param series The series code identifying the time series to delete.
   */
  void delete_series(const std::string& series);

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
