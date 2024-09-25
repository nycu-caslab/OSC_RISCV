#include "main.hpp"
#include "io.hpp"
#include "reset.hpp"
#include "uart.hpp"

void kernel_main() {
  uart_init();
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
