CXX 		:= clang++$(LLVM_SUFFIX)
LD 			:= ld.lld$(LLVM_SUFFIX)
OBJCOPY 	:= llvm-objcopy$(LLVM_SUFFIX)

CXXFLAGS 	:= 
# CXXFLAGS 	+= -march=rv64gc
CXXFLAGS 	+= --target=riscv64-unknown-none-elf
CXXFLAGS 	+= -mcmodel=medany
CXXFLAGS 	+= -nostdlib
CXXFLAGS   	+= -std=c++20
CXXFLAGS 	+= -D_LIBCPP_HAS_NO_THREADS -D_LIBCPP_DISABLE_AVAILABILITY -D_LIBCPP_CSTDLIB
CXXFLAGS 	+= -Wall -Wextra
CXXFLAGS 	+= -fno-exceptions -fno-c++-static-destructors -fno-rtti

LDFLAGS :=
LDFLAGS += --Bstatic
LDFLAGS += --discard-locals

ifeq ("$(origin O)", "command line")
OUTPUT_DIR 	:= $(O)
endif
ifndef OUTPUT_DIR
OUTPUT_DIR 	:= $(shell pwd)/build
endif

TARGET 		?= kernel
TARGET_ELF 	?= $(OUTPUT_DIR)/$(TARGET).elf
TARGET_BIN 	?= $(OUTPUT_DIR)/$(TARGET).bin

SRC_DIR 	= src
INCLUDE_DIR = include
LINKER 		:= $(SRC_DIR)/linker.ld

CXXFLAGS 	+= -I$(INCLUDE_DIR)

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
ASMS := $(shell find $(SRC_DIR) -name '*.S')
OBJS :=
OBJS += $(patsubst $(SRC_DIR)/%.cpp,$(OUTPUT_DIR)/%.o,$(SRCS))
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
	$(CXX) -MMD -MP $(CXXFLAGS) -c $< -o $@

$(OUTPUT_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) -MMD -MP $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET_BIN) $(TARGET_ELF) $(OBJS) $(DEPS)
