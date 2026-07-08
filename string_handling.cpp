/*
  This code handles strings differently from TeX. TeX does not use any Pascal string-handling
  features but instead uses a custom string pool.

  This code uses C++ std::strings and C-style null-delimited string constants. Any TeX
  code with an explicit string constant is written as a C-style double-quote-delimited string
  (or a single-quote-delimited character constant for single-character instances). Strings
  defined at runtime---typically csnames---are allocated as dynamic instances of std::string.
  TeX never deletes a string, so we use std::string* pointers freely, passing them by
  value and never deleting them. There is no need to refer to them via std::unique_ptr
  or std::shared_ptr.

  For compatibility with TeX routines, we refer to dynamically-allocated strings via
  a table of pointers.
 */

#include "string_handling.hpp"

#include <string>

class string_table str_start;

// Based on Section 48 (Initialize the first 127 entries of the string table with
// printable representations of the ASCII characters).

string_table::string_table() {
  for (int k = 0; k < 128; k++) {
    std::string* s = new std::string();
    if (k < ' ' || k > '~') {
      s->append(2, '^');
      if (k < 0100) {
        s->append(1, char(k + 0100));
      }
      else {
        s->append(1, char(k - 0100));
      }
    }
    else {
      s->append(1, char(k));
    }
    str_start.strings[k] = s;
  }
}
