CLANG_VERSION  	?=
CC 				= clang$(CLANG_VERSION)
LD 				= $(CC) -fuse-ld=lld
OBJCOPY  		= llvm-objcopy$(CLANG_VERSION)

QEMU = qemu-system-riscv64

DTB_FILE = ./disk/jh7110-starfive-visionfive-2-v1.3b.dtb

CFLAGS = -march=rv64gc \
		 --target=riscv64-unknown-none-elf \
		 -mcmodel=medany \
		 -nostdlib \
		 -Wall -Wextra

QEMUFLAGS = -M virt \
			-display none \
			-serial stdio \
			-smp cpus=4
			# -dtb $(DTB_FILE)

.PHONY: all clean build qemu
all: build qemu

build: kernel.img kernel.fit

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

kernel.fit: visionfive2-fit-image.its kernel.img $(DTB_FILE)
	mkimage -f visionfive2-fit-image.its -A riscv -O linux -T flat_dt $@

qemu:
	$(QEMU) $(QEMUFLAGS) $(QEMU_EXT_FLAGS) -kernel kernel.img
