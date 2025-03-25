/*
  Main driver program for the TimeSeries class.
  This program provides a command-line interface for interacting with
  time-series data. It supports commands to load data, print the series,
  add/update points, compute mean, check monotonicity, calculate the best-fit
  line, and exit the program.
*/
#include "../include/country_container.hpp"
#include "../include/relation.hpp"
#include <iostream>
#include <string>

int main() {
  CountryContainer container;
  std::string cmd; // Command input from the user.

  // Command processing loop.
  while (std::cin >> cmd) {
    if (cmd == "LOAD") {
      std::string fileName;
      std::cin >> fileName;
      container.load(fileName);
      std::cout << "success" << std::endl;
    } else if (cmd == "INITIALIZE") {
      container.initialize();
      std::cout << "success" << std::endl;
    } else if (cmd == "UPDATE_EDGES") {
      std::string seriesCode;
      double threshold;
      RELATION relation;
      std::cin >> seriesCode >> threshold >> relation;
      std::cout << (container.update_edges(seriesCode, threshold, relation)
                        ? "success"
                        : "failure")
                << std::endl;
    } else if (cmd == "ADJACENT") {
      std::string countryCode;
      std::cin >> countryCode;
      container.adjacent(countryCode);
    } else if (cmd == "PATH") {
      std::string countryCode1, countryCode2;
      std::cin >> countryCode1 >> countryCode2;
      std::cout << (container.path(countryCode1, countryCode2) ? "true"
                                                               : "false")
                << std::endl;
    } else if (cmd == "RELATIONSHIPS") {
      std::string countryCode1, countryCode2;
      std::cin >> countryCode1 >> countryCode2;
      container.relationships(countryCode1, countryCode2);
    } else if (cmd == "EXIT") {
      break;
    }
  }

  return 0;
}
