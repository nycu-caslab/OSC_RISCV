QEMU 		:= qemu-system-riscv64
MINICOM 	:= minicom
SERIAL 		:= /dev/cu.usbserial-0001

OUTPUT_DIR 	:= $(PWD)/build
export OUTPUT_DIR
SRC_DIR  	:= $(PWD)/src
DISK_DIR 	:= $(PWD)/disk

KERNEL_BIN 	:= $(DISK_DIR)/kernel.img

QEMUFLAGS  	:=
QEMUFLAGS 	+= -M virt
QEMUFLAGS 	+= -display none
QEMUFLAGS 	+= -serial stdio
QEMUFLAGS 	+= -smp cpus=4

.PHONY: all clean build qemu uart
all: build qemu

build: $(KERNEL_BIN)

$(KERNEL_BIN):
	${MAKE} -C $(SRC_DIR) TARGET_BIN=$(KERNEL_BIN)

clean:
	${MAKE} -C $(SRC_DIR) TARGET_BIN=$(KERNEL_BIN) clean
	$(RM) $(KERNEL_BIN)

qemu: $(KERNEL_BIN)
	$(QEMU) $(QEMUFLAGS) $(QEMU_EXT_FLAGS) -kernel $(KERNEL_BIN)

uart:
	$(MINICOM) -D $(SERIAL)
