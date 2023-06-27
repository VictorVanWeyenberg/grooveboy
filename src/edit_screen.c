#include "edit_screen.h"
#include "tracker.h"
#include "vram.h"
#include "dma.h"
#include <stdlib.h>
#include <string.h>

char *index_to_note_notation(uint8_t index) {
  char *notation = malloc(sizeof(char) * 3);
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

void update_edit_screen_notes() {
  short *text = malloc(sizeof(short) * 1024);
  memcpy(text, MEM_BG1_SCREEN_BLOCK, sizeof(short) * 1024);
  uint8_t *indexes = tracker_track_pattern_indeces(0);
  for (uint8_t y = 0; y < 16; y++) {
    for (uint8_t x = 0; x < 3; x++) {
      uint8_t index = indexes[y*3+x];
      char *notation = index_to_note_notation(index);
      for (uint8_t i = 0; i < 3; i++) {
	uint8_t sx = (x*4)+4;
	uint8_t sy = y+3;
	text[sy*32+sx+i] = ((notation[i] - 32) & 0x3FF);
      }
      free(notation);
    }
  }
  dma_push(1, text, 2048, MEM_BG1_SCREEN_BLOCK);
  dma_on(1);
  free(text);
}
