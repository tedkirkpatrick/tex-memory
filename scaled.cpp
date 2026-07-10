#include "scaled.hpp"

#include "printing.hpp"

void print_scaled(scaled s) {
  scaled delta;
  if (s < 0) {
    print_char('-');
    s = - s;
  }
  print_int(s / unity);
  print_char('.');
  s = 10 * (s % unity) + 5;
  delta = 10;
  do {
    if (delta > unity)
      s += 0x80'00 - (delta / 2);
    print_char('0' + (s / unity));
    s = 10 * (s % unity);
    delta *= 10;
  }
  while (s > delta);
}
