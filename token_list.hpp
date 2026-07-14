#ifndef TOKEN_LIST_HPP
#define TOKEN_LIST_HPP

#include "basic-memory.hpp"
#include "command_codes.hpp"

// Not in TeX but useful for testing and debugging

[[nodiscard]] constexpr halfword make_letter_token(char c) { return halfword(letter * 0x1'00 + c); }

[[nodiscard]] extern pointer new_token_list(int refc);

extern pointer add_token_to_list(pointer p, halfword token);

// Section 289

constexpr int cs_token_flag = 0x10'00;

// Section 292

extern void show_token_list(int p, int q, int l);

// Section 295

extern void token_show(pointer p);

// Section 296

extern void print_meaning();

#endif
