#include "io.hpp"

#include "uart.hpp"

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

void puts(const char* s) {
  while (*s != 0)
    putchar(*s++);
}

int readline(char* buffer, int length) {
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
