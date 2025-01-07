#include "initramfs.hpp"
#include "io.hpp"
#include "shell/cmd.hpp"
#include "video.hpp"

using namespace video;

int cmd_screen_cat(int argc, char* argv[]) {
  if (argc < 2) {
    kprintf("%s: require file\n", argv[0]);
    kprintf("usage: %s <file> [yscale] [xscale] [times]\n", argv[0]);
    return -1;
  }

  int r = 0;
  auto name = argv[1];
  size_t yscale = argc < 3 ? 20 : strtol(argv[2]);
  size_t xscale = argc < 4 ? 10 : strtol(argv[3]);
  size_t times = argc < 5 ? 4 : strtol(argv[4]);
  kprintf("name = %s / scale = %dx%d / times = %u\n", name, yscale, xscale,
          times);

  auto f = initramfs.find(name);
  if (f == nullptr) {
    r = -1;
    kprintf("%s: %s: No such file or directory\n", argv[0], name);
  } else if (f->isdir()) {
    r = -1;
    kprintf("%s: %s: Is a directory\n", argv[0], name);
  } else {
    size_t size = f->filesize();
    size_t width = 0;
    while (width < size and f->file()[width] != '\n')
      width++;
    size_t height = size / width;
    kprintf("size = %u = %ux%u\n", size, width, height);

    size_t xbeg = (xsize - width * xscale) / 2;
    size_t ybeg = (ysize - height * yscale) / 2;

    for (size_t i = 0; i < times; i++)
      for (size_t idx = 0, ix = 0, iy = 0; idx < size; idx++, ix++) {
        auto c = f->file()[idx];
        if (c == '\n') {
          iy++, ix = -1;
          continue;
        }

        if (c == ' ' or !isprint(c))
          continue;

        unsigned char red = (c * 37) % 256;
        unsigned char green = (c * 59) % 256;
        unsigned char blue = (c * 83) % 256;
        uint32_t color = (red << 16) | (green << 8) | blue;

        // kprintf("(%d,%d)=%x: \n", iy, ix, color);
        for (size_t dy = 0; dy < yscale; dy++)
          for (size_t dx = 0; dx < xscale; dx++) {
            auto y = ybeg + iy * yscale + dy;
            auto x = xbeg + ix * xscale + dx;
            // kprintf("(%d,%d)", y, x);
            fb[y * xsize + x] = color;
          }
        // kprintf("\n");
      }
  }

  return r;
}
