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

void kernel_main() {
  // FIXME
  const int cnt = 10;
  for (int i = 0; i <= cnt; i++) {
    if ((UART_GET(UART_LSR_OFFSET) & ((1 << 5) | (1 << 6))) != 0)
      break;
    if (i == cnt)
      UART_UNIT_SIZE = 4;
  }

  char s[] = "Hello world!\n\0";
  puts(s);
  for (;;) {
    putchar(getchar());
  }
}
