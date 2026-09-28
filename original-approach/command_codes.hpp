#ifndef COMMAND_CODES_HPP
#define COMMAND_CODES_HPP

/*
  Command codes in TeX.
  Part 15 of TeX: The Program
 
  See ../README.md for details of the overall project.

  COPYRIGHT
  The code in this file is a transliteration of the original Pascal TeX routines into a C-style C++.
  The file tex.web containing the original code features the following notice:

      This program is copyright (C) 1982 by D. E. Knuth; all rights are reserved.
      Unlimited copying and redistribution of this file are permitted as long
      as this file is not modified. Modifications are permitted, but only if
      the resulting file is not named tex.web

   This modification of the original file is public domain.
   See See https://creativecommons.org/publicdomain/zero/1.0/

   The copyright of the original versions remains.
*/

// Section 207

constexpr int escape = 0;
constexpr int relax = 0;
constexpr int left_brace = 1;
constexpr int right_brace = 2;
constexpr int math_shift = 3;
constexpr int tab_mark = 4;
constexpr int car_ret = 5;
constexpr int out_param = 5;
constexpr int mac_param = 6;
constexpr int sup_mark = 7;
constexpr int sub_mark = 8;
constexpr int ignore = 9;
constexpr int endv = 9;
constexpr int spacer = 10;
constexpr int letter = 11;
constexpr int other_char = 12;
constexpr int active_char = 13;
constexpr int par_end = 13;
constexpr int match = 13;
constexpr int comment = 14;
constexpr int end_match = 14;
constexpr int stop = 14;
constexpr int invalid_char = 15;
constexpr int delim_num = 15;
constexpr int max_char_code = 15;

// Section 210

constexpr int top_bot_mark = 109; // In TeX, this is defined as the result of a long calculation
constexpr int call = 110; // In TeX, this is defined as the result of a long calculation

#endif
