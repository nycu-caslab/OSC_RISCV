#include "shell.hpp"

#include "fdt.hpp"
#include "io.hpp"
#include "reset.hpp"

void shell() {
#define BUF_SIZE 1024
  char buf[BUF_SIZE];

  for (;;) {
    kputs("$ ");
    int len = kreadline(buf, BUF_SIZE);
    if (len <= 0)
      continue;
    switch (buf[0]) {
      case 'h':
        switch (buf[3]) {
          case 'p':
            kputs("help\t: print this help menu\n");
            kputs("hello\t: print Hello World!\n");
            kputs("reboot\t: reboot the device\n");
            kputs("fdt\t: print device tree\n");
            break;
          case 'l':
            kputs("Hello World!\n");
            break;
        }
        break;
      case 'r':
        reset();
        break;
      case 'f':
        fdt.print();
        break;
    }
  }
}
