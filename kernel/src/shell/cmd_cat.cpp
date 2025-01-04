#include "initramfs.hpp"
#include "io.hpp"
#include "shell/cmd.hpp"

int cmd_cat(int argc, char* argv[]) {
  int r = 0;
  if (argc == 1) {
    for (;;) {
      char c = kgetchar();
      if (c == 4)  // ^D
        break;
      kputchar(c);
    }
  } else {
    for (int i = 1; i < argc; i++) {
      auto name = argv[i];
      auto f = initramfs.find(name);
      if (f == nullptr) {
        r = -1;
        kprintf("cat: %s: No such file or directory\n", name);
      } else if (f->isdir()) {
        r = -1;
        kprintf("cat: %s: Is a directory\n", name);
      } else {
        for (auto c : f->file()) {
          kputchar(c);
        }
      }
    }
  }
  return r;
}
