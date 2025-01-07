#include "io.hpp"
#include "shell/cmd.hpp"
#include "video.hpp"

using namespace video;

int cmd_screen_color(int argc, char* argv[]) {
  if (argc < 2) {
    kprintf("%s: require color\n", argv[0]);
    kprintf("usage: %s <color>\n", argv[0]);
    return -1;
  }

  uint32_t color = strtol(argv[1]);
  kprintf("color = %x\n", color);

  for (size_t y = 0; y < ysize; y++)
    for (size_t x = 0; x < xsize; x++)
      get(x, y) = color;

  wait();

  return 0;
}
