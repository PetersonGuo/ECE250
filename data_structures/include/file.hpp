#ifndef PEEK
#define PEEK

#include <fstream>
#include <string>

inline void peek_line(std::ifstream& is, std::string& line, char dlm = '\n') {
  std::streampos pos = is.tellg(); // Save the current position
  if (std::getline(is, line, dlm))
    is.seekg(pos);
}

#endif