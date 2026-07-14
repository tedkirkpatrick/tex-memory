#include "scaled.hpp"

#include "printing.hpp"

// Section 103

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

// Section 104

bool arith_error {false};

// Section 105

scaled nx_plus_y(int n, scaled x, scaled y) {
  if (n < 0) {
    x = -x;
    n = -n;
  }
  if (n == 0)
    return y;
  else if (x <= (0x3F'FF'FF'FF - y) / n && -x <= (0x3F'FF'FF'FF + y) / n)
    return n * x + y;
  else {
    arith_error = true;
    return 0;
  }
}
