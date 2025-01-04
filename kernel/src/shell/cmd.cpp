#include "shell/cmd.hpp"

#include "io.hpp"

int runcmd(const char* buf, int len) {
  if (len <= 0)
    return 0;

  char buf_[len + 1];
  memcpy(buf_, buf, len);
  buf_[len] = 0;

  int argc = 1;
  for (int i = 0; i < len; i++)
    if (buf_[i] == ' ') {
      buf_[i] = '\0';
      argc++;
    }
  char* argv[argc];
  argv[0] = buf_;
  for (int i = 1; i < argc; i++)
    argv[i] = argv[i - 1] + strlen(argv[i - 1]) + 1;

  const Cmd* cmd = nullptr;
  for (int i = 0; i < ncmd; i++) {
    auto it = &cmds[i];
    if (!strcmp(argv[0], it->name)) {
      cmd = it;
      break;
    }
  }

  if (cmd != nullptr) {
    return cmd->fp(argc, argv);
  } else {
    kprintf("command not found: %s\n", buf);
    return -1;
  }
}
