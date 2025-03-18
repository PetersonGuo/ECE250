#include "country.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// Invalidate cache upon instantiation
Country::Country() : m_cache(0) {}

void Country::close()
{
  m_name = "";
  m_code = "";
  m_series.clear();
}

void Country::load(const std::string& country)
{
  // Clear all TimeSeries and previous data before loading new data
  close();
  std::string line;
  std::ifstream is;

  is.open("lab2_multidata.csv");
  if (is.fail()) {
    std::cout << "invalid file" << std::endl;
  }

  // Search for country code while not at end of file
  while (!is.eof()) {
    std::stringstream ss;
    std::string token, name, code;

    // Get the line of the csv and the name and country code
    std::getline(is, line, '\n');
    ss.str(line);
    std::getline(ss, name, ',');
    std::getline(ss, code, ',');

    if (name != country) // Skip load since not the correct country
      continue;

    // Set the name and country code
    m_name = name;
    m_code = code;

    // Load the TimeSeries using a stringstream that it pointing at the
    // TimeSeries name
    m_series.push_back(TimeSeries(ss));
  }
  is.close();
}

void Country::list()
{
  // Iterate through the TimeSeries's and print names
  std::cout << m_name << " " << m_code << " ";
  for (TimeSeries& t : m_series)
    std::cout << t.get_series_name() << " ";
  std::cout << std::endl;
}

void Country::add(const std::string& series, const uint32_t year,
                  const uint32_t data)
{
  // Iterate through the TimeSeries's and find the one with the matching code
  for (TimeSeries& t : m_series) {
    if (t.get_series_code() == series) {
      t.add_data_point(year, data);

      // Invalidate the cache
      m_cache = 0;
      return; // Exit once TimeSeries found and added data
    }
  }

  // No matching TimeSeries exists as function did not early exit
  std::cout << "failure" << std::endl;
}

void Country::update(const std::string& series, const uint32_t year,
                     const uint32_t data)
{
  // Iterate through TimeSeries's to find a matching code
  for (TimeSeries& t : m_series) {
    if (t.get_series_code() == series) {
      t.update_data_point(year, data);

      // Invalidate cache
      m_cache = 0;
      return; // Exit once the TimeSeries found and data was updated
    }
  }

  // No matching TimeSeries exists as function did not early exit
  std::cout << "failure" << std::endl;
}

void Country::print(const std::string& series)
{
  // Iterate through TimeSeries's to find a matching code
  for (TimeSeries& t : m_series) {
    if (t.get_series_code() == series) {
      t.print();
      return;
    }
  }

  // No matching TimeSeries exists as function did not early exit
  std::cout << "failure" << std::endl;
}

void Country::delete_series(const std::string& series)
{
  // Iterate through TimeSeries's to find a matching code
  for (TimeSeries& t : m_series) {
    if (t.get_series_code() == series) {
      m_cache =
          m_biggest != t.get_series_code(); // Invalidate cache if the deleted
                                            // series has the largest mean
      m_series.erase(
          t); // Erase the TimeSeries from the series list by reference
      std::cout << "success" << std::endl;
      return;
    }
  }

  // No matching TimeSeries exists as function did not early exit
  std::cout << "failure" << std::endl;
}

void Country::calc_biggest()
{
  // Calculate the largest mean and cache it since this is an expensive
  // function, running in O(N) best case and O(N * m) worst case
  int biggest = 0;
  m_biggest = ""; // Reset the biggest code for the case where no series exist
  for (TimeSeries& t : m_series) {
    if (t.get_size()) {
      int mean = t.mean();
      if (biggest < mean) {
        biggest = mean;
        m_biggest = t.get_series_code();
      }
    }
  }
  m_cache = 1; // Cache is now valid since largest mean was recalculated
}

void Country::biggest()
{
  // Recalculate largest mean if cache is invalid
  if (!m_cache)
    calc_biggest();

  std::cout << (m_biggest == "" ? "failure" : m_biggest) << std::endl;
}
