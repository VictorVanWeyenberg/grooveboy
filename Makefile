TARGET     := grooveboy

DEVKITPRO  := /opt/devkitpro
DEVKITARM  := $(DEVKITPRO)/devkitARM
PREFIX     := /bin/arm-none-eabi-

CC         := $(DEVKITARM)$(PREFIX)gcc
AS         := $(DEVKITARM)$(PREFIX)as
OBJCOPY    := $(DEVKITARM)$(PREFIX)objcopy
BIN2C      := $(DEVKITPRO)/tools/bin/bin2c
GBAFIX     := $(DEVKITPRO)/tools/bin/gbafix

opt := -I include -O3 -fomit-frame-pointer -marm -mcpu=arm7tdmi
CFLAGS := $(opt) -std=c11 -pedantic -Wall

BUILD_DIR  := build

MAIN_FILE  := src/$(TARGET).c
ASM_FILES  := $(wildcard asm/*.s)
SRC_FILES  := $(filter-out $(MAIN_FILE), $(wildcard src/*.c))
BIN_FILES  := $(wildcard bin/*.bin)

ASM_OFILES := $(patsubst asm/%.s,$(BUILD_DIR)/%.o,$(ASM_FILES))
SRC_OFILES := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
BIN_CFILES := $(patsubst bin/%.bin,$(BUILD_DIR)/%.c,$(BIN_FILES))
BIN_OFILES := $(BIN_CFILES:.c=.o)

OBJ_FILES  := $(ASM_OFILES) $(BIN_OFILES) $(SRC_OFILES)

all: post-build

post-build: $(TARGET).gba

pre-build:
	mkdir -p $(BUILD_DIR)

test: pre-build
	cd test && $(MAKE) test

$(TARGET).gba: pre-build $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $@
	$(GBAFIX) $@
	rm -f $(TARGET).elf

$(TARGET).elf: $(ASM_OFILES) $(BIN_OFILES) $(SRC_OFILES)
	$(CC) -I include $(OBJ_FILES) $(MAIN_FILE) -o $@ -Tlnkscript -nostartfiles -lm

$(ASM_OFILES): build/%.o: asm/%.s
	$(AS) -o $@ $<

$(BIN_OFILES): build/%.o: build/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

build/%.c: bin/%.bin 
	$(BIN2C) $< $@ data
	sed -i "s/data/$(notdir $(basename $@))/g" $@

$(SRC_OFILES): build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all pre-build post-build test clean 
clean:
	rm -f $(TARGET).gba $(TARGET).elf $(ASM_OFILES) $(SRC_OFILES) $(BIN_CFILES) $(BIN_OFILES)
	rm -rf $(BUILD_DIR)
