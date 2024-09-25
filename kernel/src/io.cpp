#include "io.hpp"

#include "nanoprintf.hpp"
#include "uart.hpp"

int kputchar(int c) {
  if (c == '\n')
    uart_write('\r');
  uart_write(c);
  return 1;
}

int kgetchar() {
  char c = uart_read();
  return c == '\r' ? '\n' : c;
}

void kputs(const char* s) {
  while (*s != 0)
    kputchar(*s++);
}

int kreadline(char* buffer, int length) {
  int r = 0;
  while (r < length) {
    char c = kgetchar();
    kputchar(c);
    if (c == '\n') {
      buffer[r++] = 0;
      break;
    }
    buffer[r++] = c;
  }
  return r;
}

void k_npf_putc(int c, void* /* ctx */) {
  kputchar(c);
}

int kvprintf(const char* format, va_list args) {
  return npf_vpprintf(&k_npf_putc, NULL, format, args);
}

int kprintf(const char* format, ...) {
  va_list args;
  va_start(args, format);
  int size = kvprintf(format, args);
  va_end(args);
  return size;
}
