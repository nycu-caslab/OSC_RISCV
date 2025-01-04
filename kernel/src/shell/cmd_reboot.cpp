#include "reset.hpp"
#include "shell/cmd.hpp"

int cmd_reboot(int /* argc */, char* /* argv */[]) {
  reset();
  return 0;
}
