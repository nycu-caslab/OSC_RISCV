#pragma once

#include "util.hpp"

// ref:
// https://github.com/torvalds/linux/blob/v6.12/arch/riscv/include/asm/sbi.h

#define SBI_SUCCESS               0
#define SBI_ERR_FAILED            -1
#define SBI_ERR_NOT_SUPPORTED     -2
#define SBI_ERR_INVALID_PARAM     -3
#define SBI_ERR_DENIED            -4
#define SBI_ERR_INVALID_ADDRESS   -5
#define SBI_ERR_ALREADY_AVAILABLE -6
#define SBI_ERR_ALREADY_STARTED   -7
#define SBI_ERR_ALREADY_STOPPED   -8

enum sbi_ext_id {
  SBI_EXT_SRST = 0x53525354,
};

enum sbi_ext_srst_fid {
  SBI_EXT_SRST_RESET = 0,
};

enum sbi_srst_reset_type {
  SBI_SRST_RESET_TYPE_SHUTDOWN = 0,
  SBI_SRST_RESET_TYPE_COLD_REBOOT,
  SBI_SRST_RESET_TYPE_WARM_REBOOT,
};

enum sbi_srst_reset_reason {
  SBI_SRST_RESET_REASON_NONE = 0,
  SBI_SRST_RESET_REASON_SYS_FAILURE,
};

struct sbiret {
  long error;
  long value;
};

struct sbiret __sbi_ecall(unsigned long arg0, unsigned long arg1,
                          unsigned long arg2, unsigned long arg3,
                          unsigned long arg4, unsigned long arg5, int fid,
                          int ext);
#define sbi_ecall(e, f, a0, a1, a2, a3, a4, a5) \
  __sbi_ecall(a0, a1, a2, a3, a4, a5, f, e)

sbiret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason);
