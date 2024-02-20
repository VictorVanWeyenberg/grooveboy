#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "dma.h"
#include "interrupt_control.h"
#include "io.h"
#include "pal.h"
#include "vram.h"
#include "tracker.h"
#include "cursor.h"
#include "edit_screen.h"
#include "timer.h"
#include "screen_coordinator.h"
#include "snd.h"
#include "lnk.h"

extern char Kick_Drumulator_DOWN;
extern int Kick_Drumulator_DOWN_length;
extern char Tom_SD_80s_Drumulator;
extern int Tom_SD_80s_Drumulator_length;

int main(void) {
  REG_DISPLAY = BG_MODE_0 | SCREEN_DISPLAY_BG0 | SCREEN_DISPLAY_BG1 |
                SCREEN_DISPLAY_BG2 | SCREEN_DISPLAY_OBJ |
                ONE_DIMENSIONAL_OBJ_CHARACTER_VRAM_MAPPING;
  BG0CNT = 0 | BGCNT_SCREEN_SIZE_0 | BG0_CHARACTER_BASE << 2 | BG0_SCREEN_BASE << 8 | 0;
  BG1CNT = 0 | BGCNT_SCREEN_SIZE_0 | BG1_CHARACTER_BASE << 2 | BG1_SCREEN_BASE << 8 | 1;
  BG2CNT = 0 | BGCNT_SCREEN_SIZE_0 | BG2_CHARACTER_BASE << 2 | BG2_SCREEN_BASE << 8 | 2;

  REG_IME = 0x00;
  REG_INTERRUPT = &interrupt_handler;
  REG_IE  = INT_DMA0 | INT_DMA1 | INT_TIMER3 | INT_TIMER2 | INT_TIMER1 | INT_COM;
  REG_IME = 0x01;

  REG_SOUNDCNT_L = 0xFF77;
  REG_SOUNDCNT_H = 0x0B0E;
  REG_SOUNDCNT_X = 0x0080;

  tracker_create();

  while(REG_VCOUNT >= 160);   // wait till VDraw
  while(REG_VCOUNT < 160);    // wait till VBlank
  snd_init();
  dma_init();
  pal_init();
  vram_init();
  init_io();

  set_screen_type(EDIT_SCREEN);
  cursor_init();

  bpm_to_start(3, 120 * tracker_patterns_rhythm_lcm()); // Sound
  bpm_to_start(2, 800); // Cursor component methods
  update_screen_lock();
  update_screen();

  dma_push(1, &Tom_SD_80s_Drumulator, Tom_SD_80s_Drumulator_length, FIFO_A);
  // timer_start(1, 52599);
  timer_start(0, 380);

  while (1) {
    // Wait for VSYNC
    while(REG_VCOUNT >= 160);   // wait till VDraw
    while(REG_VCOUNT < 160);    // wait till VBlank

    // Draw Screen
    update_screen();
    dma_cycle();

    // Prepare
    register_presses();

    uint32_t data = 0xAAAAAAAA;
    send(&data, 4);

    // Loop end
    // loop_end();
  }

  return 0;
}
