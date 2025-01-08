#include "initramfs.hpp"
#include "io.hpp"
#include "shell/cmd.hpp"
#include "video.hpp"

using namespace video;

int cmd_screen_img(int argc, char* argv[]) {
  if (argc < 2) {
    kprintf("%s: require file\n", argv[0]);
    kprintf("usage: %s <file> [scale] [times]\n", argv[0]);
    return -1;
  }

  auto name = argv[1];
  size_t scale = argc < 3 ? 10 : strtol(argv[2]);
  auto times = argc < 4 ? 1 : strtol(argv[3]);
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
    uint32_t data[];
  };
  auto s = (Img*)f->file().data();

  auto img_width = s->w;
  auto img_height = s->h;
  auto img_frames = s->f;
  kprintf("%s size = %ux%ux%u\n", name, img_width, img_height, img_frames);

  size_t xbeg = (xsize - img_width * scale) / 2;
  size_t ybeg = (ysize - img_height * scale) / 2;

  if (xbeg < 0 or ybeg < 0) {
    kprintf("%s: size exceed !!\n", argv[0]);
    return -1;
  }
  auto img_data = s->data;

  for (long t = 0; t < times; t++) {
    for (size_t frame_id = 0; frame_id < img_frames; frame_id++) {
      for (size_t y = 0; y < img_height; y++) {
        for (size_t x = 0; x < img_width; x++) {
          size_t idx = frame_id * img_width * img_height + y * img_width + x;
          uint32_t color = img_data[idx];
          for (size_t dy = 0; dy < scale; dy++)
            for (size_t dx = 0; dx < scale; dx++)
              get(xbeg + x * scale + dx, ybeg + y * scale + dy) = color;
        }
      }

      wait(1);
    }
  }

  return 0;
}
