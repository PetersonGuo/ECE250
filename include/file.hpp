#ifndef PEEK
#define PEEK

#include <fstream>
#include <string>

/*
  Peeks the next line in the input stream without consuming it.
  @param is The input stream to peek from.
  @param line The string to store the peeked line.
  @param dlm The delimiter character to use for reading the line.
*/
inline void peekline(std::ifstream& is, std::string& line, char dlm = '\n') {
  std::streampos pos = is.tellg(); // Save the current position
  if (std::getline(is, line, dlm))
    is.seekg(pos);
}

#endif