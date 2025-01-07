#define NOP asm volatile("ADDI x0, x0, 0")

#define UART_BASE       0x10000000LL
#define UART_THR_OFFSET 0
#define UART_RBR_OFFSET 0
#define UART_LSR_OFFSET 5

// int UART_UNIT_SIZE = 1;
#define UART_UNIT_SIZE 4

unsigned int UART_GET(int offset) {
  switch (UART_UNIT_SIZE) {
    case 1:
      return *(unsigned char*)(UART_BASE + offset * UART_UNIT_SIZE);
    case 4:
      return *(unsigned int*)(UART_BASE + offset * UART_UNIT_SIZE);
    default:
      __builtin_unreachable();
  }
}

void UART_SET(int offset, unsigned int value) {
  switch (UART_UNIT_SIZE) {
    case 1:
      *(unsigned char*)(UART_BASE + offset * UART_UNIT_SIZE) = value;
      return;
    case 4:
      *(unsigned int*)(UART_BASE + offset * UART_UNIT_SIZE) = value;
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

char getchar_raw() {
  return uart_read();
}

int getchar() {
  char c = getchar_raw();
  return c == '\r' ? '\n' : c;
}

void puts(const char* s) {
  while (*s != 0)
    putchar(*s++);
}

// ref:
// https://github.com/starfive-tech/opensbi/blob/VF2_6.1_v3.8.2/lib/utils/gpio/fdt_gpio_starfive.c#L55-L74

#define STARFIVE_GPIO_OUTVAL         0x40
#define STARFIVE_GPIO_MASK           0xff
#define STARFIVE_GPIO_REG_SHIFT_MASK 0x3
#define STARFIVE_GPIO_SHIFT_BITS     0x3

typedef unsigned int u32;

static inline void writel(u32 val, volatile void* addr) {
  asm volatile("sw %0, 0(%1)" : : "r"(val), "r"(addr));
}
static inline u32 readl(const volatile void* addr) {
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
  u32 val = readl((void*)(reg_addr + STARFIVE_GPIO_OUTVAL));
  val &= ~bit_mask;
  val |= value << shift_bits;
  writel(val, (void*)(reg_addr + STARFIVE_GPIO_OUTVAL));
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

char i2h(int x) {
  if (0 <= x && x <= 9)
    return '0' + x;
  if (10 <= x && x <= 15)
    return 'A' + x - 10;
  return '?';
}

void* memcpy(void* dst, const void* src, unsigned long n) {
  char* d = (char*)dst;
  const char* s = (const char*)src;
  for (unsigned long i = 0; i < n; i++) {
    *d = *s;
    if (*d != *s) {
      puts("memcpy fail !!???\n");
      while (1)
        ;
    }
    d++;
    s++;
  }
  return dst;
}

void print_hex(long value) {
  long x = 1;
  while (value / x >= 16)
    x *= 16;
  puts("0x");
  for (; x; x /= 16)
    putchar(i2h(value / x % 16));
}

void read(void* p, long size) {
  char* s = p;
  for (long i = 0; i < size; i++) {
    char c = getchar_raw();
    s[i] = c;
  }
}

void hexdump(const void* buf, unsigned size, const char* msg) {
  const char* s = (const char*)buf;
  puts(msg ? msg : __func__);
  puts(": ");
  print_hex(size);
  puts(" bytes dumped\n");
  for (unsigned i = 0; i < size; i += 16) {
    print_hex(i);
    puts(": ");
    for (unsigned j = 0; j < 16; j++) {
      if (i + j < size) {
        putchar(i2h(s[i + j] / 16));
        putchar(i2h(s[i + j] % 16));
        putchar(' ');
      } else {
        puts("   ");
      }
    }
    puts("    ");
    for (unsigned j = 0; j < 16; j++) {
      if (i + j < size) {
        if (0x20 <= s[i + j] && s[i + j] <= 0x7e)
          putchar(s[i + j]);
        else
          putchar('.');
      } else {
        puts(" ");
      }
    }
    puts("\n");
  }
  return;
}

long kernel_address = 0x80200000;
int size = 0;

void bootloader_main(int bootcore, void* dtb) {
  puts("Hello Bootloader!\n");

  print_hex((long)&bootloader_main);
  puts("\n");

  kernel_address = 0x80200000;

  for (;;) {
    puts("$ ");
    switch (getchar()) {
      case 'a':
        read(&kernel_address, sizeof(kernel_address));
        break;
      case 'p':
        puts("size = ");
        print_hex(size);
        putchar('\n');

        puts("kernel_address = ");
        print_hex(kernel_address);
        putchar('\n');
        break;
      case 's':
        read(&size, sizeof(size));

        puts("size = ");
        print_hex(size);
        putchar('\n');

        puts("kernel_address = ");
        print_hex(kernel_address);
        putchar('\n');

        puts("start read\n");
        read((void*)kernel_address, size);
        puts("end read\n");

        break;
      case 'd':
        hexdump((void*)kernel_address, size, 0);
        break;
      case 'j':
        asm volatile("fence.i\t\n");
        ((typeof(&bootloader_main))(kernel_address))(bootcore, dtb);
        break;
      case 'r':
        reset();
        break;
      default:
        puts("a\t: set kernel address\n");
        puts("p\t: print kernel address\n");
        puts("s\t: send kernel\n");
        puts("d\t: dump kernel\n");
        puts("j\t: jump to kernel\n");
        puts("r\t: reboot the device\n");
    }
  }
}
