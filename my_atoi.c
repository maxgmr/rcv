#include <ctype.h>
#include <stdbool.h>

static const int ASCII_DIGIT_OFFSET = 48;
static const int ASCII_DIGIT_END = 58;

static const int ASCII_OCT_DIGIT_END = 56;

static const int ASCII_LCASE_OFFSET = 97;
static const int ASCII_LCASE_HEX_END = 103;

static const int ASCII_UCASE_OFFSET = 65;
static const int ASCII_UCASE_HEX_END = 72;

// 0xA = 10, 0xB = 11, etc.
static const int HEX_NUM_OFFSET = 10;

// Convert a single char, representing a numerical element, to an integer.
// Return 0 if the provided char is not a valid hexadecimal digit.
//
// This function is also used to convert decimal and octal chars because decimal
// and octal character notation is a subset of hexadecimal character notation.
// It is safe to use in `atoi` because `atoi` checks to make sure that decimal
// and octal digits are within their respective ranges beforehand.
static int char_to_i(char c) {
  // 0-9
  if ((c >= ASCII_DIGIT_OFFSET) && (c < ASCII_DIGIT_END)) {
    return (int)c - ASCII_DIGIT_OFFSET;
  }

  // a-f (lowercase)
  if ((c >= ASCII_LCASE_OFFSET) && (c < ASCII_LCASE_HEX_END)) {
    return ((int)c + HEX_NUM_OFFSET) - ASCII_LCASE_OFFSET;
  }

  // A-F (uppercase)
  if ((c >= ASCII_UCASE_OFFSET) && (c < ASCII_UCASE_HEX_END)) {
    return ((int)c + HEX_NUM_OFFSET) - ASCII_UCASE_OFFSET;
  }

  // Invalid value. Should be unreachable if function used correctly.
  return 0;
}

// Convert an ASCII decimal, hexadecimal, or octal string to an integer.
//
// Matches the functionality of the standard library `atoi` implementation by
// returning 0 if the string cannot be parsed as an integer.
//
// Exhibits undefined behaviour if the given string goes outside the bounds of
// the int type.
//
// In accordance with our email communications, the signature has been modified
// from the original question such that this function takes a pointer to a
// string as a parameter rather than a single char. It also takes in the string
// as a constant to express that the original string is not modified.
int my_atoi(const char *s) {
  int result = 0;
  bool is_negative = false;

  // Discard leading whitespace
  while ((*s != '\0') && isspace(*s)) {
    s++;
  }

  // If there is a positive or negative sign, parse it and set
  // `is_negative` accordingly.
  if (*s == '+') {
    s++;
  } else if (*s == '-') {
    is_negative = true;
    s++;
  }

  // If 0x prefix, then number is hex.
  if (s[0] == '0' && ((s[1] == 'x') || (s[1] == 'X'))) {
    // Advance past prefix
    s += 2;

    // Parse number
    while ((*s != '\0') && isxdigit(*s)) {
      result *= 0x10;
      // Safe to call `char_to_i` because conditional ensured `isxdigit`,
      // meaning that *s is in the range 0x0-0xF.
      result += char_to_i(*s++);
    }
  }

  // If 0 prefix, then number is octal.
  else if (s[0] == '0') {
    // Advance past prefix
    s++;

    // Parse number
    while ((*s != '\0') && isdigit(*s) && ((int)*s < ASCII_OCT_DIGIT_END)) {
      result *= 010;
      // Safe to call `char_to_i` because conditional ensured `isdigit`
      // and below upper octal char limit, meaning that *s is not in the
      // ranges 0x8-0xF.
      result += char_to_i(*s++);
    }
  }

  // Number is decimal.
  else {
    // Parse number
    while ((*s != '\0') && isdigit(*s)) {
      result *= 10;
      // Safe to call `char_to_i` because conditional ensured `isdigit`,
      // meaning that *s is not in the range 0xA-0xF.
      result += char_to_i(*s++);
    }
  }

  // Ignore any characters following the numerical digits...

  // Apply negative.
  if (is_negative) {
    result = -result;
  }

  return result;
}
