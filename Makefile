TARGET     := grooveboy

DEVKITPRO  := /opt/devkitpro
DEVKITARM  := $(DEVKITPRO)/devkitARM
PREFIX     := /bin/arm-none-eabi-

CC         := $(DEVKITARM)$(PREFIX)gcc
OBJCOPY    := $(DEVKITARM)$(PREFIX)objcopy
LD         := $(DEVKITARM)$(PREFIX)ls
AS         := $(DEVKITARM)$(PREFIX)as
BIN2C      := bin2c
GBAFIX     := $(DEVKITPRO)/tools/bin/gbafix

MODEL      := -mthumb -mthumb-interwork -marm # -mlong-calls #This makes interrupts work
SPECS      := -specs=gba.specs
CFLAGS     := $(MODEL) -I include -Wall -pedantic -std=c99 -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -mfloat-abi=soft -ffast-math
LDFLAGS    := $(SPECS) $(MODEL) -lm --verbose

BUILD_DIR  := build

ASM_FILES  := $(wildcard asm/*.s)
SRC_FILES  := $(wildcard src/*.c)
BIN_FILES  := $(wildcard bin/*.bin)

ASM_OFILES := $(patsubst asm/%.s,$(BUILD_DIR)/%.o,$(ASM_FILES))
SRC_OFILES := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
BIN_CFILES := $(patsubst bin/%.bin,$(BUILD_DIR)/%.c,$(BIN_FILES))
BIN_OFILES := $(BIN_CFILES:.c=.o)

OBJ_FILES  := $(ASM_OFILES) $(SRC_OFILES) $(BIN_OFILES)


all: post-build

post-build: $(TARGET).gba

pre-build:
	mkdir -p $(BUILD_DIR)

bin:  pre-build $(BIN_CFILES)

test: pre-build
	cd test && $(MAKE) test

$(TARGET).gba: pre-build $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $@
	$(GBAFIX) $@
	# rm -f $(TARGET).elf

$(TARGET).elf: $(BIN_OFILES) $(SRC_OFILES) $(ASM_OFILES)
	$(CC) $(OBJ_FILES) $(LDFLAGS) -o $@ -g

$(BIN_OFILES): build/%.o: build/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build/%.c: bin/%.bin 
	$(BIN2C) $< $@ data
	sed -i "s/data/$(notdir $(basename $@))/g" $@

$(SRC_OFILES): build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -g

$(ASM_OFILES): build/%.o: asm/%.s
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all pre-build post-build test clean 
clean:
	rm -f $(TARGET).gba $(TARGET).elf $(ASM_OFILES) $(SRC_OFILES) $(BIN_CFILES) $(BIN_OFILES)
	rm -rf $(BUILD_DIR)
