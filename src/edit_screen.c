#include "edit_screen.h"
#include "edit_screen_callbacks.h"
#include "tracker.h"
#include "vram.h"
#include "dma.h"
#include "cursor.h"
#include <stdlib.h>
#include <string.h>

extern short edit_screen_BG01_screen_data;
extern short edit_screen_BG01_screen_data_length;
extern component_callback edit_screen_component_callbacks;
short *text;
uint8_t edit_screen_page = 0;

void edit_screen_init() {
  text = calloc(edit_screen_BG01_screen_data_length, sizeof(uint8_t));
}

char * index_to_note_notation(uint8_t index) {
  static char notation[3];
  uint8_t note = index % 12;
  if (note == 0 || note == 1) {
    notation[0] = 'C';
  } else if (note == 2 || note == 3) {
    notation[0] = 'D';
  } else if (note == 4) {
    notation[0] = 'E';
  } else if (note == 5 || note == 6) {
    notation[0] = 'F';
  } else if (note == 7 || note == 8) {
    notation[0] = 'G';
  } else if (note == 9 || note == 10) {
    notation[0] = 'A';
  } else if (note == 11) {
    notation[0] = 'B';
  }
  if (note == 0 || note == 2 || note == 4 ||
      note == 5 || note == 7 || note == 9 ||
      note == 11) {
    notation[1] = '-';
  } else {
    notation[1] = '#';
  }
  notation[2] = '0' + ((index / 12) - 1);
  return notation;
}

char to_hex(uint8_t value) {
  if (value >= 0 && value <= 9) {
    return value + 16;
  }
  if (value >= 10 && value <= 15) {
    return value + 23;
  }
  return ' ';
}


void update_edit_screen_notes() {
  memcpy(text, &edit_screen_BG01_screen_data, edit_screen_BG01_screen_data_length);
  uint8_t *indexes = tracker_selected_pattern_indeces();
  for (uint8_t y = 0; y < 16; y++) {
    for (uint8_t x = 0; x < 3; x++) {
      uint8_t index = indexes[y*3+x+3*16*edit_screen_page];
      char *notation = index_to_note_notation(index);
      for (uint8_t i = 0; i < 3; i++) {
        uint8_t sx = (x*4)+4;
        uint8_t sy = y+3;
        text[sy*32+sx+i] = ((notation[i] - 32) & 0x3FF);
      }
      text[38+x*4] = to_hex(tracky->instruments[x].selected_pattern);
    }
    text[(y+3)*32+1] = to_hex(edit_screen_page);
  }
  dma_push(1, text, edit_screen_BG01_screen_data_length, MEM_BG1_SCREEN_BLOCK);
  dma_on(1);
  handle_page_flag = 1;
  handle_clipboard_flag = 1;
}
