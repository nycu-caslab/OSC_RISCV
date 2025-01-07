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
  // fdt /reserved-memory
  fdt.print("/reserved-memory");
  fdt.find(
      "/reserved-memory",
      +[](uint32_t tag, int level, const char* node_name, const char* prop_name,
          uint32_t len, const char prop_value[]) -> bool {
        if (tag == FDT_PROP and !strcmp(prop_name, "reg") and
            len == sizeof(uint64_t) * 2) {
          auto addr = fdt_ld64(prop_value);
          auto size = fdt_ld64(prop_value + sizeof(uint64_t));
          kprintf("/reserved-memory/%s: %p (%x)\n", node_name, addr, size);
          mm_reserve(addr, addr + size);
        }
        return false;
      });

  mm_init();

  shell();
}
