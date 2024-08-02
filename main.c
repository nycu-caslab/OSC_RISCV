#define NOP asm volatile("ADDI x0, x0, 0")

#define UART_BASE 0x10000000
#define UART_THR (volatile unsigned int *)(UART_BASE + (0 << 2))
#define UART_RBR (volatile unsigned int *)(UART_BASE + (0 << 2))
#define UART_LSR (volatile unsigned int *)(UART_BASE + (5 << 2))

void uart_write(char c) {
  while ((*UART_LSR & ((1 << 5) | (1 << 6))) == 0)
    NOP;
  *UART_THR = c;
}

char uart_read() {
  while ((*UART_LSR & (1 << 0)) == 0)
    NOP;
  return *UART_RBR;
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

void kernel_main() {
  char s[] = "Hello world!\n\0";
  puts(s);
  for (;;) {
    putchar(getchar());
  }
}
