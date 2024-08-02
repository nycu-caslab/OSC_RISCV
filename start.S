#include "image.h"

.global _start
_start:
// ref: https://github.com/torvalds/linux/blob/v6.10/arch/riscv/kernel/head.S
// ref: https://www.kernel.org/doc/html/v5.8/riscv/boot-image-header.html
    j       _start_kernel
    .word   0
    .balign 8
    // offset 0
    .dword  0
    // size
    .dword  _end - _start
    // flags
    .dword  0
    // version
    .word   RISCV_HEADER_VERSION
    // res1
	.word   0
    // res2
	.dword  0
    // magic
	.ascii  RISCV_IMAGE_MAGIC
	.balign 4
    // magic2
	.ascii  RISCV_IMAGE_MAGIC2
    // res3
    .word   0


.global _start_kernel
_start_kernel:
    j   boot_cpu

.global prog_hang
prog_hang:
    wfi
    j   prog_hang

boot_cpu:
    la  sp, __stack_end

    call kernel_main

    j   prog_hang
