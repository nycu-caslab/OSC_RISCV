#include "sbi.hpp"

struct sbiret __sbi_ecall(unsigned long arg0, unsigned long arg1,
                          unsigned long arg2, unsigned long arg3,
                          unsigned long arg4, unsigned long arg5, int fid,
                          int ext) {
  sbiret ret;

  register uintptr_t a0 asm("a0") = (uintptr_t)(arg0);
  register uintptr_t a1 asm("a1") = (uintptr_t)(arg1);
  register uintptr_t a2 asm("a2") = (uintptr_t)(arg2);
  register uintptr_t a3 asm("a3") = (uintptr_t)(arg3);
  register uintptr_t a4 asm("a4") = (uintptr_t)(arg4);
  register uintptr_t a5 asm("a5") = (uintptr_t)(arg5);
  register uintptr_t a6 asm("a6") = (uintptr_t)(fid);
  register uintptr_t a7 asm("a7") = (uintptr_t)(ext);

  asm volatile("ecall"
               : "+r"(a0), "+r"(a1)
               : "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
               : "memory");

  ret.error = a0;
  ret.value = a1;

  return ret;
}

sbiret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason) {
  return sbi_ecall(SBI_EXT_SRST, SBI_EXT_SRST_RESET, reset_type, reset_reason,
                   0, 0, 0, 0);
}