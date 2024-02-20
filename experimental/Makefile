DEVKITARM  := $(DEVKITPRO)/devkitARM
PREFIX     := /bin/arm-none-eabi-

CC         := $(DEVKITARM)$(PREFIX)gcc
OBJCOPY    := $(DEVKITARM)$(PREFIX)objcopy
LD         := $(DEVKITARM)$(PREFIX)ls
AS         := $(DEVKITARM)$(PREFIX)as
AIF2PCM    := $(DEVKITPRO)/tools/bin/aif2pcm
GBAFIX     := $(DEVKITPRO)/tools/bin/gbafix
RAW2C     := $(DEVKITPRO)/tools/bin/raw2c

MODEL      := -mthumb -mthumb-interwork -marm # -mlong-calls #This makes interrupts work
SPECS      := -specs=gba.specs
CFLAGS     := $(MODEL) -I include -Wall -pedantic -std=c99 -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -mfloat-abi=soft -ffast-math
LDFLAGS    := $(SPECS) $(MODEL) -lm --verbose

SOUND      := China

kick.gba: kick.elf
	$(OBJCOPY) -O binary $< $@
	$(GBAFIX) $@

kick.elf: kick.o main.o
	$(CC) $^ $(LDFLAGS) -o $@ -g

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

kick.c: kick.bin
	$(RAW2C) $<

kick.bin: $(SOUND).bin
	cp $< $@

$(SOUND).bin: $(SOUND).wav
	ffmpeg -i $< -f s8 -ar 16000 $@

clean:
	rm -f *.bin kick.c kick.h *.o *.elf *.gba

# WAV to AIF using FFMPEG
# AIF to PCM using https://github.com/pret/pokeemerald/blob/master/tools/aif2pcm/main.c
# PCM to C   using raw2gba
