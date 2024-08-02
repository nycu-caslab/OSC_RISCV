#include "image.h"

.global _start
_start:
    j   boot_cpu

.global prog_hang
prog_hang:
    wfi
    j   prog_hang

boot_cpu:
    la  sp, __stack_end

    call kernel_main

    j   prog_hang
