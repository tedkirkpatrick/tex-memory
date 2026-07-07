#ifndef PRINT_HPP
#define PRINT_HPP

#include <string_view>

// Section 54

extern int tally;

// Section 59

extern void print(int sn);
extern void print(const char* s);

// Section 63

extern void print_esc(std::string_view s);

#endif
