#include "shell/args.hpp"

#include "string.hpp"

Args::Args(const char* buf_, int len) {
  /*
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
  */
}
