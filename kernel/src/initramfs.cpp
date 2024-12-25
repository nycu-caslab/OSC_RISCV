#include "initramfs.hpp"

#include "fdt.hpp"
#include "io.hpp"

CPIO initramfs;

void initramfs_init() {
  auto find32 = [](auto path) {
    auto [found, view] = fdt.find(path);
    if (!found) {
      kprintf("initramfs: device %s not found\n", path);
      return (char*)0;
    }
    auto addr = (char*)fdt_ld64(view.data());
    kprintf("initramfs: %s = %p\n", path, addr);
    return addr;
  };
  auto start = find32("/chosen/linux,initrd-start");
  auto end = find32("/chosen/linux,initrd-end");

  if (not start and not end) {
    // FIXME
    kprintf("use fixed initramfs\n");
    start = (char*)0x46100000;
    end = start + 0x2000;
  }

  kprintf("initramfs      : %p ~ %p\n", start, end);
  if (not initramfs.init(start, end)) {
    kprintf("initramfs: init failed\n");
  }
}
