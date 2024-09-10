__bootloader    = 0x46200000;
__kernel        = 0x80200000;
__stack_sz      = 0x2000;

SECTIONS
{
    . = __bootloader;

    .text : {
        KEEP(*(.text._start))
        *(.text .text.*)
    }

    .rodata : {
        *(.rodata .rodata.* .gnu.linkonce.r*)
    }

    .data : {
        *(.data .data.* .gnu.linkonce.d*)
    }

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    }

    _end = .;

    /DISCARD/ : {
        *(.comment)
        *(.gnu*) *(.note*) *(.eh_frame*)
    }

    . = ALIGN(0x1000);
    PROVIDE(__stack_beg = .);
    . += __stack_sz;
    PROVIDE(__stack_end = .);
}
