#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "dma.h"
#include "interrupt_control.h"
#include "io_control.h"
#include "pal.h"
#include "vram.h"
#include "tracker.h"
#include "edit_screen.h"

int main(void) {
  REG_DISPLAY = BG_MODE_0 | SCREEN_DISPLAY_BG0 | SCREEN_DISPLAY_BG1 |
                SCREEN_DISPLAY_BG2 | SCREEN_DISPLAY_OBJ |
                ONE_DIMENSIONAL_OBJ_CHARACTER_VRAM_MAPPING;
  BG0CNT = 0 | BGCNT_SCREEN_SIZE_0 | BG0_CHARACTER_BASE << 2 | BG0_SCREEN_BASE << 8 | 0;
  BG1CNT = 0 | BGCNT_SCREEN_SIZE_0 | BG1_CHARACTER_BASE << 2 | BG1_SCREEN_BASE << 8 | 1;
  BG2CNT = 0 | BGCNT_SCREEN_SIZE_0 | BG2_CHARACTER_BASE << 2 | BG2_SCREEN_BASE << 8 | 2;
  
  REG_IME = 0x00;
  REG_INTERRUPT = &interrupt_handler;
  REG_IE |= INT_VBLANK | INT_DMA0 | INT_DMA1;
  REG_IME = 0x01;

  dma_init();
  pal_init();
  vram_init();

  edit_screen_init();
  tracker_create();
  update_edit_screen_notes();
  
  while (1);
  return 0;
}
