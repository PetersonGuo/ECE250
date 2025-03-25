#ifndef RELATION_HPP
#define RELATION_HPP

#include <iostream>

enum RELATION {
  EQUAL,
  LESS,
  GREATER,
};

inline std::ostream& operator<<(std::ostream& out, const RELATION value) {
  return out << (value == EQUAL ? "equal" : value == LESS ? "less" : "greater");
}

inline std::istream& operator>>(std::istream& in, RELATION& value) {
  std::string input;
  in >> input;
  if (input == "equal")
    value = EQUAL;
  else if (input == "less")
    value = LESS;
  else if (input == "greater")
    value = GREATER;
  return in;
}

#endif