#pragma once

extern "C" {
extern char __stack_end[];

void _start();
void kernel_main(int hartid, void* fdt);
}
