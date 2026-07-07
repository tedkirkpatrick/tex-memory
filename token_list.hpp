#ifndef TOKEN_LIST_HPP
#define TOKEN_LIST_HPP

#include "basic-memory.hpp"

// Section 289

constexpr int cs_token_flag = 0x10'00;

// Section 292

extern void show_token_list(int p, int q, int l);

// Section 295

extern void token_show(pointer p);

// Section 296

extern void print_meaning();

#endif
