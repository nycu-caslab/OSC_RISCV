.section ".text._start"
#define __bootloader 0x46200000

.global _start
_start:
    j   boot_cpu

.global prog_hang
prog_hang:
    wfi
    j       prog_hang

boot_cpu:
    la      sp, __stack_end

    mv      s1, a0
    mv      s2, a1

    li      t1, __bootloader
    la      t0, _start
    bne     t0, t1, relocate

    call    bootloader_main

    j       prog_hang

relocate:
    li      a0, __bootloader
    la      a1, _start
    la      t1, _end
    sub     a2, t1, a1
    jal     memcpy

    // restore boot core
    mv      a0, s1
    // restore dtb addr
    mv      a1, s2
    li      t0, __bootloader
    jr      t0

    j       prog_hang
