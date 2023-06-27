#ifndef __VRAM_H
#define __VRAM_H

#define MEM_VRAM 0x06000000

#define BG0_SCREEN_BASE 29
#define BG1_SCREEN_BASE 30
#define BG2_SCREEN_BASE 31

#define MEM_BG0_SCREEN_BLOCK ((short*)(MEM_VRAM + BG0_SCREEN_BASE * 0x800))
#define MEM_BG1_SCREEN_BLOCK ((short*)(MEM_VRAM + BG1_SCREEN_BASE * 0x800))
#define MEM_BG2_SCREEN_BLOCK ((short*)(MEM_VRAM + BG2_SCREEN_BASE * 0x800))

#define BG0_CHARACTER_BASE 0
#define BG1_CHARACTER_BASE 1
#define BG2_CHARACTER_BASE 2

#define MEM_BG0_CHARACTER_BLOCK ((short*)(MEM_VRAM + BG0_CHARACTER_BASE * 0x4000))
#define MEM_BG1_CHARACTER_BLOCK ((short*)(MEM_VRAM + BG1_CHARACTER_BASE * 0x4000))
#define MEM_BG2_CHARACTER_BLOCK ((short*)(MEM_VRAM + BG2_CHARACTER_BASE * 0x4000))

#define OBJ_CHARACTER_BLOCK ((short*)(MEM_VRAM + 0x10000))

void vram_init();
void play_screen_init();
void edit_screen_init();

#endif
