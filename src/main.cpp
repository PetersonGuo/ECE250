/*
  Main driver program for the TimeSeries class.
  This program provides a command-line interface for interacting with
  time-series data. It supports commands to load data, print the series,
  add/update points, compute mean, check monotonicity, calculate the best-fit
  line, and exit the program.
*/

#include "../include/country_container.hpp"
#include <iostream>
#include <string>

int main() {
  CountryContainer container;
  std::string cmd; // Command input from the user.

  // Command processing loop.
  while (std::cin >> cmd) {
    if (cmd == "LOAD") {
      // Load data from a file.
      std::string fileName;
      std::cin >> fileName;
      container.load(fileName);
      std::cout << "success" << std::endl;
    } else if (cmd == "LIST") {
      std::string countryName;
      std::cin >> countryName;
      container.list(countryName);
    } else if (cmd == "RANGE") {
      std::string seriesCode;
      std::cin >> seriesCode;
      Pair<double, double> p = container.range(seriesCode);
      std::cout << p.getFirst() << ' ' << p.getSecond() << '\n';
    } else if (cmd == "BUILD") {
      std::string seriesCode;
      std::cin >> seriesCode;
      container.build(seriesCode);
      std::cout << "success\n";
    } else if (cmd == "FIND") {
      int op;
      double mean;
      std::string operation;
      std::cin >> mean >> operation;
      op = operation == "less" ? -1 : operation == "equal" ? 0 : 1;
      container.find(mean, op);
    } else if (cmd == "DELETE") {
      // Delete a Country
      std::string countryName;
      std::cin >> countryName;
      std::cout << (container.delete_country(countryName) ? "success" : "failure") << '\n';
    } else if (cmd == "LIMITS") {
      std::string condition;
      std::cin >> condition;
      container.limits(condition == "lowest");
    } else if (cmd == "EXIT") {
      // Exit the program.
      return 0;
    }
  }

  return 0;
}
