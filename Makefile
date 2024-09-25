QEMU 		:= qemu-system-riscv64
MINICOM 	:= minicom
SERIAL 		:= /dev/cu.usbserial-0001

KERNEL_SRC  := $(PWD)/src
BOOT_SRC  	:= $(PWD)/bootloader
DISK_DIR 	:= $(PWD)/disk

KERNEL_BIN 	:= $(DISK_DIR)/kernel.img
BOOTLOADER 	:= $(DISK_DIR)/bootloader.img

QEMUFLAGS  	:=
QEMUFLAGS 	+= -M virt
QEMUFLAGS 	+= -display none
QEMUFLAGS 	+= -serial stdio
QEMUFLAGS 	+= -smp cpus=4
QEMUFLAGS 	+= -m 4G

.PHONY: all clean kernel bootloader build qemu uart
all: build qemu

build: kernel

kernel: $(KERNEL_BIN)
$(KERNEL_BIN): FORCE
	${MAKE} -C $(KERNEL_SRC) TARGET_BIN=$(KERNEL_BIN)

bootloader: $(BOOTLOADER)
$(BOOTLOADER): FORCE
	${MAKE} -C $(BOOT_SRC) TARGET_BIN=$(BOOTLOADER)

clean:
	${MAKE} -C $(KERNEL_SRC) TARGET_BIN=$(KERNEL_BIN) clean
	${MAKE} -C $(BOOT_SRC) TARGET_BIN=$(BOOTLOADER) clean
	$(RM) $(KERNEL_BIN)

qemu: $(KERNEL_BIN)
	$(QEMU) $(QEMUFLAGS) $(QEMU_EXT_FLAGS) -kernel $(KERNEL_BIN)

qemu-bootloader: $(BOOTLOADER)
	$(QEMU) $(QEMUFLAGS) $(QEMU_EXT_FLAGS) -kernel $(BOOTLOADER)

uart:
	$(MINICOM) -D $(SERIAL)

FORCE:
