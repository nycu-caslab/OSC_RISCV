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
  while (true) {
    char c = kgetchar();
    switch (c) {
      case 8:     // ^H
      case 0x7f:  // backspace
        if (r > 0) {
          buffer[r--] = 0;
          kputchar('\b');
          kputchar(' ');
          kputchar('\b');
        }
        break;
      case 0x15:  // ^U
        while (r > 0) {
          buffer[r--] = 0;
          kputchar('\b');
          kputchar(' ');
          kputchar('\b');
        }
        break;
      case '\n':
        kputchar('\n');
        buffer[r] = '\0';
        goto end;
      case '\t':  // skip \t
        break;
      default:
        if (r + 1 < length) {
          buffer[r++] = c;
          kputchar(c);
        }
    }
  }
end:
  return r;
}

void kprint_hex(string_view view) {
  for (auto c : view)
    kprintf("%02x", c);
}
void kprint_str(string_view view) {
  for (auto c : view)
    kputchar(c);
}

void kprint(string_view view) {
  if (view.printable())
    kprint_str(view);
  else
    kprint_hex(view);
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
