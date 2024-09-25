QEMU 		:= qemu-system-riscv64
MINICOM 	:= minicom
SERIAL 		:= /dev/cu.usbserial-0001

KERNEL_SRC  := $(PWD)/kernel
BOOT_SRC  	:= $(PWD)/bootloader
DISK_DIR 	:= $(PWD)/disk
FS_DIR 		:= $(PWD)/rootfs

KERNEL_BIN 	:= $(DISK_DIR)/kernel.img
BOOTLOADER 	:= $(DISK_DIR)/bootloader.img
INITFSCPIO 	:= $(DISK_DIR)/initramfs.cpio

QEMUFLAGS  	:=
QEMUFLAGS 	+= -M virt
QEMUFLAGS 	+= -display none
QEMUFLAGS 	+= -serial stdio
QEMUFLAGS 	+= -smp cpus=4
QEMUFLAGS 	+= -m 4G
QEMUFLAGS 	+= -initrd $(INITFSCPIO)

.PHONY: all clean kernel bootloader build qemu uart fs
all: build qemu

build: kernel

kernel: $(KERNEL_BIN)
$(KERNEL_BIN): FORCE
	${MAKE} -C $(KERNEL_SRC) TARGET_BIN=$(KERNEL_BIN)

bootloader: $(BOOTLOADER)
$(BOOTLOADER): FORCE
	${MAKE} -C $(BOOT_SRC) TARGET_BIN=$(BOOTLOADER)

fs: $(INITFSCPIO)
$(INITFSCPIO): $(shell find $(FS_DIR))
	# $(MAKE) -C $(FS_DIR)
	cd $(FS_DIR) && find . | grep -v '.DS_Store' | cpio -o -H newc > $@

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

upload:
	./upload.py $(SERIAL)

FORCE:
