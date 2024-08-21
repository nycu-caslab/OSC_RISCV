CLANG_VERSION  	?=
CC 				= clang$(CLANG_VERSION)
LD 				= $(CC) -fuse-ld=lld
OBJCOPY  		= llvm-objcopy$(CLANG_VERSION)
QEMU 			= qemu-system-riscv64


CFLAGS 		= -march=rv64gc \
		  	  --target=riscv64-unknown-none-elf \
		  	  -mcmodel=medany \
		  	  -nostdlib \
		  	  -Wall -Wextra

QEMUFLAGS  	= -M virt \
			  -display none \
			  -serial stdio \
			  -smp cpus=4

.PHONY: all clean build qemu
all: build qemu

build: disk/kernel.img

clean:
	$(RM) -r disk/kernel.img build

disk/kernel.img: build/kernel.elf
	$(OBJCOPY) -O binary $< $@

build/kernel.elf: src/linker.ld build/start.o build/main.o
	$(LD) -T src/linker.ld $(CFLAGS) build/start.o build/main.o -o $@

build/start.o: src/start.S
	@mkdir -p $(@D)
	$(CC) -MMD -MP $(CFLAGS) -c $< -o $@

build/main.o: src/main.c
	@mkdir -p $(@D)
	$(CC) -MMD -MP $(CFLAGS) -c $< -o $@

qemu:
	$(QEMU) $(QEMUFLAGS) $(QEMU_EXT_FLAGS) -kernel disk/kernel.img
