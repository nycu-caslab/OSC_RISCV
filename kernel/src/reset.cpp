#include "reset.hpp"

#include "io.hpp"
#include "sbi.hpp"
#include "starfive.hpp"

void gpio_reset() {
  const int offset = 35;
  long addr = 0x13040000;
  starfive_gpio_set(addr, offset, 1);
}

void spi_reset() {
  sbi_system_reset(SBI_SRST_RESET_TYPE_WARM_REBOOT, SBI_SRST_RESET_REASON_NONE);
}

void reset() {
  kputs("resetting ... \n");

  // gpio_reset();
  spi_reset();

  for (;;)
    kputchar('.');
}
