/*
  Main driver program for the TimeSeries class.
  This program provides a command-line interface for interacting with
  time-series data. It supports commands to load data, print the series,
  add/update points, compute mean, check monotonicity, calculate the best-fit
  line, and exit the program.
*/

#include "time_series.hpp"
#include <iostream>

// Enum representing the set of available commands.
enum commands { LOAD, PRINT, ADD, UPDATE, MEAN, MONOTONIC, FIT, EXIT };

int main(int argc, char *argv[]) {
  TimeSeries time; // TimeSeries object to manage time-series data.
  std::string cmd; // Command input from the user.

  // Command processing loop.
  while (std::cin >> cmd) {
    if (cmd == "LOAD") {
      // Load data from a file.
      std::string filename;
      std::cin >> filename;
      time.load_data(filename);
    } else if (cmd == "PRINT") {
      // Print all data points in the series.
      time.print();
    } else if (cmd == "ADD") {
      // Add a new data point to the series.
      int year, data;
      std::cin >> year >> data;
      time.add_data_point(year, data);
    } else if (cmd == "UPDATE") {
      // Update an existing data point in the series.
      int year, data;
      std::cin >> year >> data;
      time.update_data_point(year, data);
    } else if (cmd == "MEAN") {
      // Calculate and display the mean value of the series.
      time.mean();
    } else if (cmd == "MONOTONIC") {
      // Check and display whether the series is monotonic.
      time.is_monotonic();
    } else if (cmd == "FIT") {
      // Calculate and display the best-fit line parameters.
      double m, b;
      time.best_fit(m, b);
    } else if (cmd == "EXIT") {
      // Exit the program.
      return 0;
    }
  }

  return 0;
}
