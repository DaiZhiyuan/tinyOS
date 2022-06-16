# Set Compiler flags
SFLAGS = -mcpu=$(CPU) -fpic -ffreestanding -nostdlib -nostartfiles
CFLAGS = -O2 -Wall -Wextra -g
LDFLAGS = -ffreestanding -O2 -nostdlib

# Set armstub name
ARMSTUB_NAME = armstub-new
ARCH_DIR = arch/armv8-a
SFLAGS += -mgeneral-regs-only
CPU = cortex-a72
ARMGNU = aarch64-none-elf
IMG_NAME = kernel8

# Set Build / Source / Include directories
BUILD_DIR = build
SRC_DIR = src
INC_DIR = include

KER_SRC = src/kernel
COMMON_SRC = src/common

KERNEL_VERSION = 0.1.0

.PHONY: clean all release cscope

all: build

# Compile project
## Compile every asm file in /arch/$(ARCH_DIR)
$(BUILD_DIR)/%_s.o: $(ARCH_DIR)/%.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(SFLAGS) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

## Compile every C file in /src/kernel
$(BUILD_DIR)/%_c.o: $(KER_SRC)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(SFLAGS) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

## Compile every C file in /src/common
$(BUILD_DIR)/%_c.o: $(COMMON_SRC)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(SFLAGS) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

## Find all object files (from corresponding C, asm files)
ASM_FILES = $(wildcard $(ARCH_DIR)/*.S)
KER_C_FILES = $(wildcard $(KER_SRC)/*.c)
COMMON_C_FILES = $(wildcard $(COMMON_SRC)/*.c)

OBJ_FILES = $(ASM_FILES:$(ARCH_DIR)/%.S=$(BUILD_DIR)/%_s.o)
OBJ_FILES += $(KER_C_FILES:$(KER_SRC)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(COMMON_C_FILES:$(COMMON_SRC)/%.c=$(BUILD_DIR)/%_c.o)

## Link all object files and create final image
build: $(OBJ_FILES)
	$(ARMGNU)-gcc -T $(ARCH_DIR)/linker.ld -o $(IMG_NAME).elf $(LDFLAGS) $^
	$(ARMGNU)-objcopy -O binary $(IMG_NAME).elf $(IMG_NAME).img

# Compile armstub
STUB_DIR = armstub
STUB_FILES = $(wildcard ${STUB_DIR}/$(SRC_DIR)/*.S)
OBJ_STUB_FILES = $(STUB_FILES:${STUB_DIR}/$(SRC_DIR)/%.S=${STUB_DIR}/$(BUILD_DIR)/%_s.o)

## Compile armstub in /armstub/src
$(STUB_DIR)/$(BUILD_DIR)/%_s.o: $(STUB_FILES)
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(SFLAGS) $(CFLAGS) -c $< -o $@

armstub: $(OBJ_STUB_FILES)
	$(ARMGNU)-ld --section-start=.text=0 -o ${STUB_DIR}/$(BUILD_DIR)/armstub.elf $<
	$(ARMGNU)-objcopy ${STUB_DIR}/$(BUILD_DIR)/armstub.elf -O binary $(ARMSTUB_NAME).bin

release: build armstub
	tar -czvf RasPi4-aarch64-$(KERNEL_VERSION).tar.gz $(IMG_NAME).img $(IMG_NAME).elf \
	$(ARMSTUB_NAME).bin config.txt 

cscope:
	@find -iname "*.S" -o -iname "*.c" -o -iname "*.h"  > cscope.files
	@cscope -bkq -i cscope.files

tags:
	@ctags -R

# Clean rules
clean:
	@rm -rf $(BUILD_DIR) *.img *.elf *.bin *.tar.gz cscope.* tags $(STUB_DIR)/$(BUILD_DIR)
