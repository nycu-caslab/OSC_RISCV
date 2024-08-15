#define NOP asm volatile("ADDI x0, x0, 0")

#define UART_BASE 0x10000000LL
#define UART_THR_OFFSET 0
#define UART_RBR_OFFSET 0
#define UART_LSR_OFFSET 5

int UART_UNIT_SIZE = 1;

unsigned int UART_GET(int offset) {
  switch (UART_UNIT_SIZE) {
  case 1:
    return *(unsigned char *)(UART_BASE + offset * UART_UNIT_SIZE);
  case 4:
    return *(unsigned int *)(UART_BASE + offset * UART_UNIT_SIZE);
  default:
    __builtin_unreachable();
  }
}

void UART_SET(int offset, unsigned int value) {
  switch (UART_UNIT_SIZE) {
  case 1:
    *(unsigned char *)(UART_BASE + offset * UART_UNIT_SIZE) = value;
    return;
  case 4:
    *(unsigned int *)(UART_BASE + offset * UART_UNIT_SIZE) = value;
    return;
  default:
    __builtin_unreachable();
  }
}

void uart_write(char c) {
  while ((UART_GET(UART_LSR_OFFSET) & ((1 << 5) | (1 << 6))) == 0)
    NOP;
  UART_SET(UART_THR_OFFSET, c);
}

char uart_read() {
  while ((UART_GET(UART_LSR_OFFSET) & (1 << 0)) == 0)
    NOP;
  return UART_GET(UART_RBR_OFFSET);
}

int putchar(int c) {
  if (c == '\n')
    uart_write('\r');
  uart_write(c);
  return 1;
}

int getchar() {
  char c = uart_read();
  return c == '\r' ? '\n' : c;
}

void puts(const char *s) {
  while (*s != 0)
    putchar(*s++);
}

int readline(char *buffer, int length) {
  int r = 0;
  while (r < length) {
    char c = getchar();
    putchar(c);
    if (c == '\n') {
      buffer[r++] = 0;
      break;
    }
    buffer[r++] = c;
  }
  return r;
}

// ref:
// https://github.com/starfive-tech/opensbi/blob/VF2_6.1_v3.8.2/lib/utils/gpio/fdt_gpio_starfive.c#L55-L74

#define STARFIVE_GPIO_OUTVAL 0x40
#define STARFIVE_GPIO_MASK 0xff
#define STARFIVE_GPIO_REG_SHIFT_MASK 0x3
#define STARFIVE_GPIO_SHIFT_BITS 0x3

typedef unsigned int u32;

static inline void writel(u32 val, volatile void *addr) {
  asm volatile("sw %0, 0(%1)" : : "r"(val), "r"(addr));
}
static inline u32 readl(const volatile void *addr) {
  u32 val;
  asm volatile("lw %0, 0(%1)" : "=r"(val) : "r"(addr));
  return val;
}

void starfive_gpio_set(long addr, long offset, int value) {
  unsigned long reg_addr = addr + offset;
  reg_addr &= ~(STARFIVE_GPIO_REG_SHIFT_MASK);

  u32 shift_bits = (offset & STARFIVE_GPIO_REG_SHIFT_MASK)
                   << STARFIVE_GPIO_SHIFT_BITS;
  u32 bit_mask = STARFIVE_GPIO_MASK << shift_bits;
  /* set output value */
  u32 val = readl((void *)(reg_addr + STARFIVE_GPIO_OUTVAL));
  val &= ~bit_mask;
  val |= value << shift_bits;
  writel(val, (void *)(reg_addr + STARFIVE_GPIO_OUTVAL));
}

void reset() {
  puts("resetting ... \n");

  const int offset = 35;
  long addr = 0x13040000;
  starfive_gpio_set(addr, offset, 1);

  for (;;) {
    putchar('.');
  }
}

void kernel_main() {
  // FIXME
  const int cnt = 10;
  for (int i = 0; i <= cnt; i++) {
    if ((UART_GET(UART_LSR_OFFSET) & ((1 << 5) | (1 << 6))) != 0)
      break;
    if (i == cnt)
      UART_UNIT_SIZE = 4;
  }

  puts("Hello Kernel!\n");

#define BUF_SIZE 1024
  char buf[BUF_SIZE];

  for (;;) {
    puts("$ ");
    int len = readline(buf, BUF_SIZE);
    if (len <= 0)
      continue;
    switch (buf[0]) {
    case 'h':
      switch (buf[3]) {
      case 'p':
        puts("help\t: print this help menu\n");
        puts("hello\t: print Hello World!\n");
        puts("reboot\t: reboot the device\n");
        break;
      case 'l':
        puts("Hello World!\n");
        break;
      }
      break;
    case 'r':
      reset();
      break;
    }
  }
}
