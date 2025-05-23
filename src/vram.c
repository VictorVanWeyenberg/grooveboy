#include "vram.h"
#include "dma.h"

extern short BG00_character_data;
extern short BG00_character_data_length;
extern short BG01_character_data;
extern short BG01_character_data_length;
extern short BG04_character_data;
extern short BG04_character_data_length;

void vram_init() {
  dma_push(1, &BG00_character_data, BG00_character_data_length, MEM_BG0_CHARACTER_BLOCK);
  dma_push(1, &BG01_character_data, BG01_character_data_length, MEM_BG1_CHARACTER_BLOCK);
  dma_push(1, &BG04_character_data, BG04_character_data_length, OBJ_CHARACTER_BLOCK);
  dma_on(1);
}

void bg_screen_write(uint8_t background_number, void *screen_data, uint16_t screen_data_length) {
  if (background_number == 0) {
    dma_push(1, screen_data, screen_data_length, MEM_BG0_SCREEN_BLOCK);
  } else if (background_number == 1) {
    dma_push(1, screen_data, screen_data_length, MEM_BG1_SCREEN_BLOCK);
  } else if (background_number == 2) {
    dma_push(1, screen_data, screen_data_length, MEM_BG2_SCREEN_BLOCK);
  }
} 
