#include "shell/cmd.hpp"

#include "fdt.hpp"
#include "initramfs.hpp"
#include "io.hpp"
#include "reset.hpp"

int runcmd(const char* buf, int len) {
  switch (buf[0]) {
    case 'h':
      switch (buf[3]) {
        case 'p':
          kputs("help\t: print this help menu\n");
          kputs("hello\t: print Hello World!\n");
          kputs("reboot\t: reboot the device\n");
          kputs("fdt\t: print device tree\n");
          kputs("ls\t: list directory contents\n");
          kputs("cat\t: concatenate and print files\n");
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
    case 'l':
      for (auto it : initramfs) {
        kprintf("%c\t%d\t%s\n", "-d"[it->isdir()], it -> filesize(),
                it -> name_ptr());
      }
      break;
    case 'c': {
      auto name = buf + 4;
      auto f = initramfs.find(name);
      if (f == nullptr) {
        kprintf("cat: %s: No such file or directory\n", name);
      } else if (f->isdir()) {
        kprintf("cat: %s: Is a directory\n", name);
      } else {
        for (auto c : f->file())
          kputchar(c);
      }
      break;
    }
  }
  return 0;
}
