#include "edit_screen.h"
#include "edit_screen_callbacks.h"
#include "tracker.h"
#include "vram.h"
#include "dma.h"
#include "cursor.h"
#include <stdlib.h>
#include <string.h>

extern uint16_t edit_screen_BG01_screen_data;
extern uint16_t edit_screen_BG01_screen_data_length;
uint8_t edit_screen_page = 0;
enum edit_mode mode;

char *HLD = "HLD";
char *$$$ = "---";

void index_to_note_notation(uint8_t index, char *notation) {
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
}

void index_to_number_notation(uint8_t index, char *notation) {
  notation[0] = ((index % 1000) / 100) + '0';
  notation[1] = ((index % 100) / 10) + '0';
  notation[2] = (index % 10) + '0';
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

void to_long_hex(uint8_t index, char *notation) {
  notation[0] = to_hex((index % 0x1000) / 0x100);
  notation[1] = to_hex((index % 0x100) / 0x10);
  notation[2] = to_hex(index % 0x10);
}

void update_edit_screen_notes() {
  uint16_t *text = calloc(edit_screen_BG01_screen_data_length / 2, sizeof(uint16_t));
  memcpy(text, &edit_screen_BG01_screen_data, edit_screen_BG01_screen_data_length);
  uint8_t *indexes = calloc(NUMBER_OF_INSTRUMENTS * NOTES_PER_PATTERN, sizeof(uint8_t));
  if (mode == NOTE) {
    tracker_selected_pattern_indeces(indexes);
  } else if (mode == HOLD) {
    tracker_selected_pattern_lengths(indexes);
  } else if (mode == DECAY) {
    tracker_selected_pattern_envelope_steps(indexes);
  } else if (mode == AMPLITUDE) {
    tracker_selected_pattern_volumes(indexes);
  } else {
    tracker_selected_pattern_indeces(indexes);
  }
  for (volatile uint8_t component_y = 0; component_y < 16; component_y++) {
    for (volatile uint8_t component_x = 0; component_x < NUMBER_OF_INSTRUMENTS; component_x++) {
      uint8_t index = indexes[NUMBER_OF_INSTRUMENTS*(component_y+16*edit_screen_page)+component_x];
      char *notation = calloc(3, sizeof(char));
      if (mode == NOTE) {
        if (get_note_of_instrument(component_x, component_y + 16 * edit_screen_page)->enabled == 0) {
          memcpy(notation, "---", 3);
        } else {
          index_to_note_notation(index, notation);
        }
      } else if (mode == DECAY && index == 0) {
        memcpy(notation, "HLD", 3);
      } else {
        index_to_number_notation(index, notation);
      }
      for (uint8_t j = 0; j < 3; j++) {
        uint8_t sx = (component_x*4)+4;
        uint8_t sy = component_y+3;
        text[sy*32+sx+j] = ((notation[j] - 32) & 0x3FF);
      }
      free(notation);
    }
    text[(component_y+3)*32+1] = to_hex(edit_screen_page);
  }
  for (uint8_t iter = 0; iter < NUMBER_OF_INSTRUMENTS; iter++) {
    uint8_t selected_pattern = tracker_instrument_selected_pattern(iter);
    text[38+iter*4] = to_hex(selected_pattern);
    text[(6+iter)*32+22] = to_hex(selected_pattern);
    uint8_t selected_pattern_length = tracker_instrument_selected_pattern_length(iter);
    char *notation = calloc(3, sizeof(char));
    to_long_hex(selected_pattern_length, notation);
    for (uint8_t iter2 = 0; iter2 < 3; iter2++) {
      text[(6+iter)*32+26+iter2] = notation[iter2];
    }
    free(notation);
  }
  if (paste_mode == NOTES) {
    text[32+28] = ' ' - 32;
  } else {
    text[32+28] = '@' - 32;
  }
  // memcpy(MEM_BG1_SCREEN_BLOCK, text, edit_screen_BG01_screen_data);
  dma_push(1, text, edit_screen_BG01_screen_data_length, MEM_BG1_SCREEN_BLOCK);
  handle_page_flag = 1;
  handle_clipboard_flag = 1;
  free(indexes);
  // free(text);
}

void set_edit_mode(enum edit_mode set_mode) {
  mode = set_mode;
}
