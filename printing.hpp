#ifndef PRINTING_HPP
#define PRINTING_HPP

#include <sstream>
#include <string_view>

// The following variables and routines and not from TeX

extern void set_cout();

extern void set_str(std::ostringstream* ostr);

// Section 54

extern int tally;

// Section 58

extern void print_char(char c);

// Section 59

extern void print(int sn);
extern void print(const char* s);

// Section 62

extern void print_ln();

// Section 63

extern void print_esc(std::string_view s);

// Section 65

extern void print_int(int n);

// Section 67

extern void print_hex(int n);

// Section 68

extern void print_ASCII(int c);

// Section 70

constexpr void print_current_string() { return; } // Stub

#endif
