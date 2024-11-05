__kernel    = 0x80200000;
__stack_sz  = 0x2000;

SECTIONS
{
    . = __kernel;

    .text : {
        KEEP(*(.text._start))
        *(.text .text.*)
    }

    .rodata : {
        *(.rodata .rodata.* .gnu.linkonce.r*)
    }

    . = ALIGN(0x1000);

    .data : {
        *(.data .data.* .gnu.linkonce.d*)
    }

    PROVIDE(__bss_start = .);
    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    }
    PROVIDE(__bss_end = .);

    . = ALIGN(0x1000);

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
