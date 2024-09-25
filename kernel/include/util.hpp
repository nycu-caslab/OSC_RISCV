#pragma once

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
