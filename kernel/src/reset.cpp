#include "reset.hpp"

#include "io.hpp"
#include "starfive.hpp"

void reset() {
  puts("resetting ... \n");

  const int offset = 35;
  long addr = 0x13040000;
  starfive_gpio_set(addr, offset, 1);

  for (;;) {
    putchar('.');
  }
}
