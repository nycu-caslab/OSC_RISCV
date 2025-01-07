CC 			:= clang$(LLVM_SUFFIX)
LD 			:= ld.lld$(LLVM_SUFFIX)
OBJCOPY 	:= llvm-objcopy$(LLVM_SUFFIX)

CFLAGS 	:= 
CFLAGS 	+= -march=rv64gc
CFLAGS 	+= --target=riscv64-unknown-none-elf
CFLAGS 	+= -mcmodel=medany
CFLAGS 	+= -nostdlib
CFLAGS 	+= -Wall
CFLAGS 	+= -Wextra
CFLAGS 	+= -g

LDFLAGS :=
LDFLAGS += --Bstatic
LDFLAGS += --discard-locals

OUTPUT_DIR 	:= $(shell pwd)/build

TARGET 		?= bootloader
TARGET_ELF 	?= $(OUTPUT_DIR)/$(TARGET).elf
TARGET_BIN 	?= $(OUTPUT_DIR)/$(TARGET).bin

SRC_DIR 	= .
LINKER 		:= $(SRC_DIR)/linker.ld

SRCS := $(shell find $(SRC_DIR) -name '*.c')
ASMS := $(shell find $(SRC_DIR) -name '*.S')
OBJS :=
OBJS += $(patsubst $(SRC_DIR)/%.c,$(OUTPUT_DIR)/%.o,$(SRCS))
OBJS += $(patsubst $(SRC_DIR)/%.S,$(OUTPUT_DIR)/%-asm.o,$(ASMS))
DEPS := $(OBJS:%.o=%.d)

.PHONY: all clean build
all: build

build: $(TARGET_BIN)

-include $(DEPS)

$(TARGET_ELF): $(LINKER) $(OBJS)
	$(LD) -T $(LINKER) $(LDFLAGS) $(OBJS) -o $@

$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@

$(OUTPUT_DIR)/%-asm.o: $(SRC_DIR)/%.S
	@mkdir -p $(@D)
	$(CC) -MMD -MP $(CFLAGS) -c $< -o $@

$(OUTPUT_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -MMD -MP $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET_BIN) $(TARGET_ELF) $(OBJS) $(DEPS)
