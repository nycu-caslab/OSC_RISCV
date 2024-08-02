#define NOP asm volatile("ADDI, x0, x0, 0")

#define UART_BASE 0x10000000
#define UART_THR (volatile unsigned int *)(UART_BASE + (0 << 2))
#define UART_LSR (volatile unsigned int *)(UART_BASE + (5 << 2))

int has_bit(unsigned int value, int bit) { return ((value >> bit) & 1) == 1; }

void uart_write(char c) {
  // while (has_bit(*UART_LSR, 5))
  //   NOP;
  *UART_THR = c;
}

int putchar(int c) {
  if (c == '\n')
    uart_write('\r');
  uart_write(c);
  return 1;
}

void puts(const char *s) {
  while (*s)
    putchar(*s++);
}

void kernel_main() {
  char s[] = "Hello world!\n";
  puts(s);
}
