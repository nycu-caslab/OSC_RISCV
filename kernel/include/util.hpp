#pragma once

// ref: https://github.com/v8/v8/blob/12.5.71/src/base/compiler-specific.h#L26
#if defined(__GNUC__)
#define PRINTF_FORMAT(format_param, dots_param) \
  __attribute__((format(printf, format_param, dots_param)))
#else
#define PRINTF_FORMAT(format_param, dots_param)
#endif

#define NOP asm volatile("ADDI x0, x0, 0")

typedef unsigned int u32;

static inline void writel(u32 val, volatile void* addr) {
  asm volatile("sw %0, 0(%1)" : : "r"(val), "r"(addr));
}
static inline u32 readl(const volatile void* addr) {
  u32 val;
  asm volatile("lw %0, 0(%1)" : "=r"(val) : "r"(addr));
  return val;
}
