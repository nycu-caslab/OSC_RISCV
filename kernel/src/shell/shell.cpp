#include "shell/shell.hpp"

#include "io.hpp"
#include "shell/cmd.hpp"

void shell() {
#define BUF_SIZE 1024
  char buf[BUF_SIZE];

  for (;;) {
    kputs("$ ");
    int len = kreadline(buf, BUF_SIZE);
    if (len <= 0)
      continue;

    runcmd(buf, len);
  }
}
