/*
  Pair class header file.
  This file defines a template-based Pair class that encapsulates two values of
  potentially different types. It provides constructors, assignment operators,
  comparison operators, and access methods for the encapsulated values.
*/

#ifndef PAIR_HPP
#define PAIR_HPP

/*
  Template-based Pair class.
  Encapsulates two values (first and second) of potentially different types and
  provides functionality for accessing, comparing, and assigning the values.

  @tparam T1 Type of the first value.
  @tparam T2 Type of the second value.
*/
template <typename T1, typename T2>
class Pair {
private:
  T1 first;  // First value of the pair.
  T2 second; // Second value of the pair.

public:
  /**
   * @brief Default constructor.
   * Initializes both values to their default states.
   */
  Pair();

  /**
   * @brief Parameterized constructor.
   * Initializes the pair with the given values.
   * @param first Value for the first element.
   * @param second Value for the second element.
   */
  Pair(const T1&, const T2&);

  /**
   * @brief Copy constructor.
   * Creates a new Pair by copying the values from another Pair.
   * @param other The Pair object to copy from.
   */
  Pair(const Pair&);

  /**
   * @brief Move constructor.
   * Creates a new Pair by transferring ownership of the values from another
   * Pair.
   * @param other The Pair object to move from.
   */
  Pair(Pair&&);

  /**
   * @brief Copy assignment operator.
   * Copies the values from another Pair into this Pair.
   * @param other The Pair object to copy from.
   * @return Reference to the updated Pair object.
   */
  Pair& operator=(const Pair&) noexcept;

  /**
   * @brief Move assignment operator.
   * Transfers the values from another Pair into this Pair.
   * @param other The Pair object to move from.
   * @return Reference to the updated Pair object.
   */
  Pair& operator=(Pair&&) noexcept;

  /**
   * @brief Equality operator.
   * Compares two Pair objects for equality based on their elements.
   * @param other The Pair object to compare with.
   * @return True if the pairs are equal, false otherwise.
   */
  bool operator==(const Pair&) const;

  /**
   * @brief Inequality operator.
   * Compares two Pair objects for inequality based on their elements.
   * @param other The Pair object to compare with.
   * @return True if the pairs are not equal, false otherwise.
   */
  bool operator!=(const Pair&) const;

  /**
   * @brief Accessor for the first element of the Pair.
   * @return Reference to the first element.
   */
  T1& getFirst();
  const T1& getFirst() const;

  /**
   * @brief Accessor for the second element of the Pair.
   * @return Reference to the second element.
   */
  T2& getSecond();
  const T2& getSecond() const;
};

#include "pair.tpp"

#endif // PAIR_HPP
