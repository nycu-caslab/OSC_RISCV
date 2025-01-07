#pragma once

#include "util.hpp"

namespace video {

inline auto fb = (volatile uint32_t*)0x00000000fe000000;
constexpr size_t fb_size = 0x2000000;
constexpr size_t xsize = 1920;
constexpr size_t ysize = 1080;
constexpr size_t bpix = 5;

}  // namespace video
