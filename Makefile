CLANG_VERSION  	?=
CC 				= clang$(CLANG_VERSION)
LD 				= $(CC) -fuse-ld=lld
OBJCOPY  		= llvm-objcopy$(CLANG_VERSION)

QEMU = qemu-system-riscv64

CFLAGS = -march=rv64gc \
		 --target=riscv64-unknown-none-elf \
		 -mcmodel=medany \
		 -nostdlib

QEMUFLAGS = -M virt \
			-display none \
			-serial stdio \
			-smp cpus=4
			# -dtb ./disk/jh7110-starfive-visionfive-2-v1.3b.dtb

.PHONY: all clean build qemu
all: qemu

build: kernel.img

clean:
	$(RM) kernel.img kernel.elf start.o main.o

kernel.img: kernel.elf
	$(OBJCOPY) -O binary $< $@

kernel.elf: linker.ld start.o main.o
	$(LD) -T linker.ld $(CFLAGS) start.o main.o -o $@

start.o: start.S
	$(CC) -MMD -MP $(CFLAGS) -c $< -o $@

main.o: main.c
	$(CC) -MMD -MP $(CFLAGS) -c $< -o $@

qemu: build
	$(QEMU) $(QEMUFLAGS) $(QEMU_EXT_FLAGS) -kernel kernel.img
