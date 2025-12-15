/** @file roman.h
 *  @brief Roman numeral conversion utility functions.
 *
 *  This header file contains declarations for converting between
 *  Arabic integers and Roman numeral strings.
 */

#ifndef ROMAN_H
#define ROMAN_H

/**
 * @mainpage Guess Number Game
 *
 * @section intro_sec Introduction
 * This is a simple number guessing game that demonstrates internationalization (i18n)
 * and documentation generation using Doxygen.
 *
 * @section usage_sec Usage
 *
 * @verbatim
 * Usage: guess [OPTIONS]
 *
 * Options:
 *   -r         Use Roman numerals mode (I, II, III...) instead of Arabic (1, 2, 3...)
 *   --help     Display this help and exit
 *   --version  Display version information and exit
 * @endverbatim
 *
 * @section roman_sec Roman Numerals
 * In Roman numeral mode, the game operates with numbers from 1 to 100.
 * Supported symbols: I, V, X, L, C.
 *
 * @author Your Name
 * @date 2025
 */

/**
 * @brief Converts an Arabic number to a Roman numeral string.
 *
 * Converts an integer between 1 and 3999 (inclusive) to its Roman numeral representation.
 *
 * @param num The integer to convert.
 * @param buf Output buffer to store the Roman string.
 * @param buf_size Size of the output buffer.
 * @return 0 on success, -1 if the number is out of range or buffer is too small.
 */
int to_roman(int num, char *buf, size_t buf_size);

/**
 * @brief Converts a Roman numeral string to an Arabic number.
 *
 * Parses a string containing a Roman numeral and returns its integer value.
 *
 * @param str The Roman numeral string.
 * @return The integer value (1-3999), or -1 if the string format is invalid.
 */
int from_roman(const char *str);

#endif
