#include <stdlib.h>
#include "tracker.h"

struct tracker *tracky;

void tracker_create() {
  free(tracky);
  tracky = malloc(sizeof(struct tracker));
  for (uint8_t n_instr = 0; n_instr < NUMBER_OF_INSTRUMENTS; n_instr++) {
    struct instrument instr;
    instr.selected_pattern = 0;
    for (uint8_t n_patt = 0; n_patt < PATTERNS_PER_INSTRUMENT; n_patt++) {
      struct pattern patt;
      for (uint8_t n_note = 0; n_note < NOTES_PER_PATTERN; n_note++) {
        struct note no;
        no.index = 36;
        no.length = 32;
        no.envelope_step = 7;
        no.direction = 1;
        no.volume = 15;
        patt.notes[n_note] = no;
      }
      instr.patterns[n_patt] = patt;
    }
    tracky->instruments[n_instr] = instr;
  }

  /* for (uint8_t n_track = 0; n_track < NUMBER_OF_TRACKS; n_track++) {
    for (uint8_t n_patt = 0; n_patt < NUMBER_OF_INSTRUMENTS; n_patt++) {
      t->tracks[n_track].patterns[n_patt] = 0;
    }
    } */
}

void tracker_change_note(uint8_t instrument, uint8_t pattern, uint8_t note_index, int8_t offset) {
  uint8_t current = tracky->instruments[instrument].patterns[pattern].notes[note_index].index;
  uint8_t offsetted = current + offset;
  if (offsetted < 36 || offsetted > 96) {
    return;
  }
  tracky->instruments[instrument].patterns[pattern].notes[note_index].index = offsetted;
}

uint8_t tracker_instrument_selected_pattern(uint8_t instrument) {
  return tracky->instruments[instrument].selected_pattern;
}

uint8_t *tracker_track_pattern_indeces(uint8_t track) {
  // Each instrument plays one pattern at a time.
  // The number of tracks, also the number of instruments.
  uint8_t *patterns = tracky->tracks[track].patterns;
  static uint8_t indeces[NUMBER_OF_INSTRUMENTS * NOTES_PER_PATTERN];
  for (uint8_t inst = 0; inst < NUMBER_OF_INSTRUMENTS; inst++) {
    struct instrument instr = tracky->instruments[inst];
    struct pattern p = instr.patterns[patterns[inst]];
    for (uint8_t index = 0; index < NOTES_PER_PATTERN; index++) {
      indeces[index * 3 + inst] = p.notes[index].index;
    }
  }
  return indeces;
}

uint8_t *tracker_selected_pattern_indeces() {
  static uint8_t indeces[NUMBER_OF_INSTRUMENTS * NOTES_PER_PATTERN];
  for (uint8_t inst = 0; inst < NUMBER_OF_INSTRUMENTS; inst++) {
    struct instrument instr = tracky->instruments[inst];
    struct pattern p = instr.patterns[instr.selected_pattern];
    for (uint8_t index = 0; index < NOTES_PER_PATTERN; index++) {
      indeces[index*3 + inst] = p.notes[index].index;
    }
  }
  return indeces;
}

uint8_t *tracker_track_pattern_lengths(uint8_t track) {
  uint8_t *patterns = tracky->tracks[track].patterns;
  uint8_t *lengths = malloc(sizeof(uint8_t) * NUMBER_OF_INSTRUMENTS * NOTES_PER_PATTERN);
  for (uint8_t inst = 0; inst < NUMBER_OF_INSTRUMENTS; inst++) {
    struct instrument instr = tracky->instruments[inst];
    struct pattern p = instr.patterns[patterns[inst]];
    for (uint8_t index = 0; index < NOTES_PER_PATTERN; index++) {
      lengths[index * 3 + inst] = p.notes[index].length;
    }
  }
  return lengths;
}

void tracker_change_selected_pattern(uint8_t instrument_number, int8_t offset) {
  struct instrument instrument = tracky->instruments[instrument_number];
  int8_t selected_pattern = instrument.selected_pattern;
  int8_t soon_selected_pattern = selected_pattern + offset;
  if (soon_selected_pattern < 0 || soon_selected_pattern >= PATTERNS_PER_INSTRUMENT) {
    return;
  }
  tracky->instruments[instrument_number].selected_pattern = soon_selected_pattern;
}
