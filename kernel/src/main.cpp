#include "main.hpp"

#include "io.hpp"
#include "shell.hpp"
#include "uart.hpp"

void kernel_main() {
  uart_init();
  kprintf("Hello Kernel!\n");
  kprintf("kernel_main @ %p\n", kernel_main);

  shell();
}
