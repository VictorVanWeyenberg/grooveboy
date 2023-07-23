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
enum edit_mode mode;

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

char * index_to_number_notation(uint8_t index) {
  static char notation[3];
  notation[0] = ((index % 1000) / 100) + '0';
  notation[1] = ((index % 100) / 10) + '0';
  notation[2] = (index % 10) + '0';
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

char * to_long_hex(uint8_t index) {
  static char notation[3];
  notation[0] = to_hex((index % 0x1000) / 0x100);
  notation[1] = to_hex((index % 0x100) / 0x10);
  notation[2] = to_hex(index % 0x10);
  return notation;
}

void update_edit_screen_notes() {
  memcpy(text, &edit_screen_BG01_screen_data, edit_screen_BG01_screen_data_length);
  uint8_t *indexes;
  if (mode == NOTE) {
    indexes = tracker_selected_pattern_indeces();
  } else if (mode == HOLD) {
    indexes = tracker_selected_pattern_lengths();
  } else if (mode == DECAY) {
    indexes = tracker_selected_pattern_envelope_steps();
  } else if (mode == AMPLITUDE) {
    indexes = tracker_selected_pattern_volumes();
  } else {
    indexes = tracker_selected_pattern_indeces();
  }
  for (uint8_t y = 0; y < 16; y++) {
    for (uint8_t x = 0; x < 3; x++) {
      uint8_t index = indexes[y*3+x+3*16*edit_screen_page];
      char *notation;
      if (mode == NOTE) {
        notation = index_to_note_notation(index);
      } else if (mode == HOLD && index == 63) {
        notation = "HLD";
      } else {
        notation = index_to_number_notation(index);
      }
      for (uint8_t i = 0; i < 3; i++) {
        uint8_t sx = (x*4)+4;
        uint8_t sy = y+3;
        text[sy*32+sx+i] = ((notation[i] - 32) & 0x3FF);
      }
    }
    text[(y+3)*32+1] = to_hex(edit_screen_page);
  }
  for (uint8_t iter = 0; iter < NUMBER_OF_INSTRUMENTS; iter++) {
    text[38+iter*4] = to_hex(tracky->instruments[iter].selected_pattern);
    text[(6+iter)*32+22] = to_hex(tracky->instruments[iter].selected_pattern);
    uint8_t selected_pattern = tracky->instruments[iter].selected_pattern;
    uint8_t selected_pattern_length = tracky->instruments[iter].patterns[selected_pattern].length;
    char *notation = to_long_hex(selected_pattern_length);
    for (uint8_t iter2 = 0; iter2 < 3; iter2++) {
      text[(6+iter)*32+26+iter2] = notation[iter2];
    }
  }
  if (paste_mode == NOTES) {
    text[32+28] = ' ' - 32;
  } else {
    text[32+28] = '@' - 32;
  }
  dma_push(1, text, edit_screen_BG01_screen_data_length, MEM_BG1_SCREEN_BLOCK);
  dma_on(1);
  handle_page_flag = 1;
  handle_clipboard_flag = 1;
}

void set_edit_mode(enum edit_mode set_mode) {
  mode = set_mode;
}
