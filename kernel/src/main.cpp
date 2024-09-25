#include "main.hpp"

#include "fdt.hpp"
#include "initramfs.hpp"
#include "io.hpp"
#include "shell.hpp"
#include "uart.hpp"

void kernel_main(int hartid, void* dtb_addr) {
  uart_init();
  kprintf("Hello Kernel!\n");
  kprintf("kernel_main @ %p\n", kernel_main);
  kprintf("boot core   = %d\n", hartid);
  kprintf("devicetree  = %p\n", dtb_addr);

  fdt.init(dtb_addr);
  initramfs_init();

  shell();
}
