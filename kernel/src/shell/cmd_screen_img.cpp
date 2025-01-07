#include "initramfs.hpp"
#include "io.hpp"
#include "shell/cmd.hpp"
#include "video.hpp"

using namespace video;

/*  Call this macro repeatedly.  After each use, the pixel data can be extracted
 */

#define HEADER_PIXEL(data, pixel)                                       \
  {                                                                     \
    pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4));         \
    pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
    pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33)));      \
    data += 4;                                                          \
  }

int cmd_screen_img(int argc, char* argv[]) {
  if (argc < 2) {
    kprintf("%s: require file\n", argv[0]);
    kprintf("usage: %s <file> [times]\n", argv[0]);
    return -1;
  }

  auto name = argv[1];
  auto times = argc < 3 ? 1 : strtol(argv[2]);
  auto f = initramfs.find(name);
  if (f == nullptr) {
    kprintf("%s: %s: No such file or directory\n", argv[0], name);
    return -1;
  } else if (f->isdir()) {
    kprintf("%s: %s: Is a directory\n", argv[0], name);
    return -1;
  }

  struct Img {
    uint32_t w, h, f;
    char data[];
  };
  auto s = (Img*)f->file().data();

  auto img_width = s->w;
  auto img_height = s->h;
  auto img_frames = s->f;
  kprintf("%s size = %ux%ux%u\n", name, img_width, img_height, img_frames);
  if (img_width > xsize or img_height > ysize) {
    kprintf("%s: size exceed !!\n", argv[0]);
    return -1;
  }
  auto img_data = s->data;

  char pixel[4];

  for (long t = 0; t < times; t++) {
    for (size_t frame_id = 0; frame_id < img_frames; frame_id++) {
      char* data = img_data + frame_id * img_width * img_height * 4;

      size_t xbeg = (xsize - img_width) / 2;
      size_t ybeg = (ysize - img_height) / 2;

      for (size_t y = 0; y < img_height; y++) {
        for (size_t x = 0; x < img_width; x++) {
          HEADER_PIXEL(data, pixel);
          uint32_t color = (pixel[0] << 16 | pixel[1] << 8 | pixel[2]);
          get(xbeg + x, ybeg + y) = color;
        }
      }

      wait();
    }
  }

  return 0;
}
