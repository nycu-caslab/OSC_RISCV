.section ".text._start"

.global _start
_start:
    j       boot_cpu

.global prog_hang
prog_hang:
    wfi
    j       prog_hang

boot_cpu:
    mv      s0, a0
    mv      s1, a1

    la      sp, __stack_end
    la      t0, __bss_start
    la      t1, __bss_end

clear_bss:
    bge     t0, t1, run_kernel
    sd      zero, 0(t0)
    addi    t0, t0, 8
    j       clear_bss

run_kernel:
    mv      a0, s0
    mv      a1, s1
    call    kernel_main

    j       prog_hang
