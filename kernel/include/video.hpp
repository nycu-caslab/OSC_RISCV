#pragma once

#include "util.hpp"

namespace video {

inline auto fb = (volatile uint32_t*)0x00000000fe000000;
constexpr size_t fb_size = 0x2000000;
constexpr size_t xsize = 1920;
constexpr size_t ysize = 1080;
constexpr size_t bpix = 5;

inline volatile uint32_t& get(uint32_t x, uint32_t y) {
  return fb[y * xsize + x];
}

inline int wait() {
  int r = 0;
  for (size_t i = 0; i < 4; i++)
    for (size_t y = 0; y < ysize; y++)
      for (size_t x = 0; x < xsize; x++)
        if (i % 2)
          r += get(x, y);
        else
          r -= get(x, y);
  return r;
}

}  // namespace video
