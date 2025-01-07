#include "main.hpp"

#include "fdt.hpp"
#include "initramfs.hpp"
#include "io.hpp"
#include "mm/mm.hpp"
#include "shell/shell.hpp"
#include "uart.hpp"

void kernel_main(int hartid, void* dtb_addr) {
  uart_init();
  kprintf("Hello Kernel!\n");
  kprintf("kernel_main @ %p\n", kernel_main);
  kprintf("boot core   = %d\n", hartid);
  kprintf("devicetree  = %p\n", dtb_addr);

  fdt.init(dtb_addr);

  mm_preinit();
  initramfs_init();

  // TODO: spin tables for multicore boot ?
  // mm_reserve(0x0000, 0x1000);
  // kernel code & bss & kernel stack
  mm_reserve(_start, __stack_end);
  // initramfs
  mm_reserve(initramfs.startp(), initramfs.endp());
  // flatten device tree
  mm_reserve(fdt.startp(), fdt.endp());

  // mm_init();

  shell();
}
