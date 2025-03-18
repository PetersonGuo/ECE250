/*
  Main driver program for the TimeSeries class.
  This program provides a command-line interface for interacting with
  time-series data. It supports commands to load data, print the series,
  add/update points, compute mean, check monotonicity, calculate the best-fit
  line, and exit the program.
*/

#include "country.hpp"
#include <cstdio>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
  Country country; // TimeSeries object to manage time-series data.
  std::string cmd; // Command input from the user.

  // Command processing loop.
  while (std::cin >> cmd) {
    if (cmd == "LOAD") {
      // Load data from a file.
      std::string countryName;
      std::getchar();
      std::getline(std::cin, countryName, '\n');
      country.load(countryName);
      std::cout << "success" << std::endl;
    }
    else if (cmd == "LIST") {
      // List TimeSeries names
      country.list();
    }
    else if (cmd == "ADD") {
      // Add data point to a TimeSeries
      std::string seriesCode;
      int year, data;
      std::cin >> seriesCode >> year >> data;
      country.add(seriesCode, year, data);
    }
    else if (cmd == "UPDATE") {
      // Update a data point in a TimeSeries
      std::string seriesCode;
      int year, data;
      std::cin >> seriesCode >> year >> data;
      country.update(seriesCode, year, data);
    }
    else if (cmd == "PRINT") {
      // Print data in a TimeSeries
      std::string seriesCode;
      std::cin >> seriesCode;
      country.print(seriesCode);
    }
    else if (cmd == "DELETE") {
      // Delete a TimeSeries
      std::string seriesCode;
      std::cin >> seriesCode;
      country.delete_series(seriesCode);
    }
    else if (cmd == "BIGGEST") {
      // Find the biggest TimeSeries mean
      country.biggest();
    }
    else if (cmd == "EXIT") {
      // Exit the program.
      return 0;
    }
  }

  return 0;
}
