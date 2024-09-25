#include "starfive.hpp"

#include "util.hpp"

// ref:
// https://github.com/starfive-tech/opensbi/blob/VF2_6.1_v3.8.2/lib/utils/gpio/fdt_gpio_starfive.c#L55-L74

#define STARFIVE_GPIO_OUTVAL         0x40
#define STARFIVE_GPIO_MASK           0xff
#define STARFIVE_GPIO_REG_SHIFT_MASK 0x3
#define STARFIVE_GPIO_SHIFT_BITS     0x3

void starfive_gpio_set(long addr, long offset, int value) {
  unsigned long reg_addr = addr + offset;
  reg_addr &= ~(STARFIVE_GPIO_REG_SHIFT_MASK);

  u32 shift_bits = (offset & STARFIVE_GPIO_REG_SHIFT_MASK)
                   << STARFIVE_GPIO_SHIFT_BITS;
  u32 bit_mask = STARFIVE_GPIO_MASK << shift_bits;
  /* set output value */
  u32 val = readl((void*)(reg_addr + STARFIVE_GPIO_OUTVAL));
  val &= ~bit_mask;
  val |= value << shift_bits;
  writel(val, (void*)(reg_addr + STARFIVE_GPIO_OUTVAL));
}
