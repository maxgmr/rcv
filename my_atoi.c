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
//
// Return 0 and set pointer ok to false if value is invalid.
static int char_to_i(char c, bool *ok) {
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
  *ok = false;
  return 0;
}

// Convert an ASCII decimal, hexadecimal, or octal string to an integer.
//
// Return 0 & set ok to false if an error occurs.
//
// Exhibits undefined behaviour if the given string goes outside the bounds of
// the int type.
int my_atoi(const char *s, bool *ok) {
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
      result += char_to_i(*s++, ok);
    }
  }

  // If 0b prefix, then number is binary.
  else if (s[0] == '0' && ((s[1] == 'b') || (s[1] == 'B'))) {
    // Advance past prefix
    s += 2;

    // Parse number
    while (*s == '0' || *s == '1') {
      result *= 2;
      // Safe to call `char_to_i` because conditional ensured *s is 0
      // or 1.
      result += char_to_i(*s++, ok);
    }
  }

  // If 0 or 0o prefix, then number is octal.
  else if (s[0] == '0') {
    // Advance past prefix
    s++;
    // Optional 'o'
    if (*s == 'o') {
      s++;
    }

    // Parse number
    while ((*s != '\0') && isdigit(*s) && ((int)*s < ASCII_OCT_DIGIT_END)) {
      result *= 010;
      // Safe to call `char_to_i` because conditional ensured `isdigit`
      // and below upper octal char limit, meaning that *s is not in the
      // ranges 0x8-0xF.
      result += char_to_i(*s++, ok);
    }
  }

  // Number is decimal.
  else {

    // Set ok to false if no digits
    if (!isdigit(*s)) {
      *ok = false;
    }

    // Parse number
    while ((*s != '\0') && isdigit(*s)) {
      result *= 10;
      // Safe to call `char_to_i` because conditional ensured `isdigit`,
      // meaning that *s is not in the range 0xA-0xF.
      result += char_to_i(*s++, ok);
    }
  }

  // Ignore any characters following the numerical digits...

  // Apply negative.
  if (is_negative) {
    result = -result;
  }

  return result;
}
