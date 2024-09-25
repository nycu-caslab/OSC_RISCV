#pragma once

// ref: https://github.com/v8/v8/blob/12.5.71/src/base/compiler-specific.h#L26
#if defined(__GNUC__)
#define PRINTF_FORMAT(format_param, dots_param) \
  __attribute__((format(printf, format_param, dots_param)))
#else
#define PRINTF_FORMAT(format_param, dots_param)
#endif

#define NOP asm volatile("ADDI x0, x0, 0")

using u32 = unsigned int;
using u64 = unsigned long long;
using addr_t = volatile char*;

static inline void writel(u32 val, volatile void* addr) {
  asm volatile("sw %0, 0(%1)" : : "r"(val), "r"(addr));
}
static inline u32 readl(const volatile void* addr) {
  u32 val;
  asm volatile("lw %0, 0(%1)" : "=r"(val) : "r"(addr));
  return val;
}

template <u64 sz, bool up = true, typename T>
inline T align(T p) {
  return (T)(((u64)p + (up ? sz - 1 : 0)) & ~(sz - 1));
}
template <typename T>
inline T align(T p, u64 sz) {
  return (T)(((u64)p + sz - 1) & ~(sz - 1));
}

extern "C" {
// start.S
[[noreturn]] void prog_hang();
}
