#include "io.hpp"
#include "shell/cmd.hpp"
#include "video.hpp"

using namespace video;

int cmd_screen_color(int argc, char* argv[]) {
  if (argc < 2) {
    kprintf("%s: require color\n", argv[0]);
    kprintf("usage: %s <color> [times]\n", argv[0]);
    return -1;
  }

  uint32_t color = strtol(argv[1]);
  size_t times = argc < 3 ? 4 : strtol(argv[2]);
  kprintf("color = %x / times = %u\n", color, times);

  for (size_t i = 0; i < times; i++)
    for (size_t y = 0; y < ysize; y++)
      for (size_t x = 0; x < xsize; x++)
        fb[y * xsize + x] = color;

  return 0;
}
