#include <stdlib.h>
#include "tracker.h"

volatile struct tracker *tracky;

void tracker_create() {
  // free(tracky);
  tracky = calloc(1, sizeof(struct tracker));
  for (uint8_t n_instr = 0; n_instr < NUMBER_OF_INSTRUMENTS; n_instr++) {
    struct instrument instr;
    instr.selected_pattern = 0;
    for (uint8_t n_patt = 0; n_patt < PATTERNS_PER_INSTRUMENT; n_patt++) {
      struct pattern patt;
      patt.length = 16;
      patt.rhythm = 0;
      for (uint8_t n_note = 0; n_note < NOTES_PER_PATTERN; n_note++) {
        struct note no;
        no.index = 36;
        no.length = 8;
        no.envelope_step = 1;
        no.enabled = 0; // n_note % 4 == 0 && n_instr == 3;
        no.volume = 15;
        patt.notes[n_note] = no;
      }
      instr.patterns[n_patt] = patt;
    }
    tracky->instruments[n_instr] = instr;
  }

  for (uint8_t n_track = 0; n_track < NUMBER_OF_TRACKS; n_track++) {
    for (uint8_t n_patt = 0; n_patt < NUMBER_OF_INSTRUMENTS; n_patt++) {
      tracky->tracks[n_track].patterns[n_patt] = 0;
    }
  }

  tracky->play_pause = 0;
}

void tracker_change_note(uint8_t instrument, uint8_t pattern, uint8_t note_index, int8_t offset) {
  volatile struct note *note = &(tracky->instruments[instrument].patterns[pattern].notes[note_index]);
  uint8_t current = note->index;
  uint8_t offsetted = current + offset;
  if (offsetted < 36 || offsetted > 96) {
    return;
  }
  note->index = offsetted;
  if (note->enabled == 0) {
    note->enabled = 1;
  }
}

void tracker_change_length(uint8_t instrument, uint8_t pattern, uint8_t note_index, int8_t offset) {
  uint8_t current = tracky->instruments[instrument].patterns[pattern].notes[note_index].length;
  int8_t offsetted = current + offset;
  if (offsetted < 0 || offsetted > 63) {
    return;
  }
  tracky->instruments[instrument].patterns[pattern].notes[note_index].length = offsetted;
}

void tracker_change_envelope_duty(uint8_t instrument, uint8_t pattern, uint8_t note_index, int8_t offset) {
  uint8_t current = tracky->instruments[instrument].patterns[pattern].notes[note_index].envelope_step;
  int8_t offsetted = current + offset;
  if (offsetted < 0 || offsetted > 7) {
    return;
  }
  tracky->instruments[instrument].patterns[pattern].notes[note_index].envelope_step = offsetted;
}

void tracker_change_volume(uint8_t instrument, uint8_t pattern, uint8_t note_index, int8_t offset) {
  uint8_t current = tracky->instruments[instrument].patterns[pattern].notes[note_index].volume;
  int8_t offsetted = current + offset;
  if (offsetted < 0 || offsetted > 15) {
    return;
  }
  tracky->instruments[instrument].patterns[pattern].notes[note_index].volume = offsetted;
}

void tracker_note_toggle_enable(uint8_t instrument, uint8_t pattern, uint8_t note_index) {
  tracky->instruments[instrument].patterns[pattern].notes[note_index].enabled =
    ~(tracky->instruments[instrument].patterns[pattern].notes[note_index].enabled);
}

void tracker_change_selected_pattern_length(uint8_t instrument_index, int8_t offset) {
  uint8_t selected_pattern = tracky->instruments[instrument_index].selected_pattern;
  uint8_t current_length = tracky->instruments[instrument_index].patterns[selected_pattern].length;
  int16_t offsetted = current_length + offset;
  if (offsetted <= 0) {
    offsetted = 1;
  }
  if (offsetted >= NOTES_PER_PATTERN - 1) {
    offsetted = NOTES_PER_PATTERN - 1;
  }
  tracky->instruments[instrument_index].patterns[selected_pattern].length = offsetted;
}

void tracker_change_selected_pattern_rhythm(uint8_t instrument_index, int8_t offset) {
  uint8_t selected_pattern = tracky->instruments[instrument_index].selected_pattern;
  uint8_t current_rhythm = tracky->instruments[instrument_index].patterns[selected_pattern].rhythm;
  int16_t offsetted = current_rhythm + offset;
  if (offsetted <= 0) {
    offsetted = 0;
  }
  if (offsetted >= 7) {
    offsetted = 7;
  }
  tracky->instruments[instrument_index].patterns[selected_pattern].rhythm = offsetted;
}

uint16_t gcd(uint16_t a, uint16_t b) {
  if (b == 0) {
        return a;
  }
  return gcd(b, a % b);
}

uint16_t clcm(uint16_t a, uint16_t b) {
  return (a / gcd(a, b)) * b;
}

uint16_t tracker_patterns_rhythm_lcm() {
  uint16_t lcm = clcm(tracker_instrument_selected_pattern_rhythm(0) + 1,
                      tracker_instrument_selected_pattern_rhythm(1) + 1);
  lcm = clcm(lcm, tracker_instrument_selected_pattern_rhythm(2) + 1);
  lcm = clcm(lcm, tracker_instrument_selected_pattern_rhythm(3) + 1);
  return lcm;
}

uint8_t tracker_instrument_selected_pattern(uint8_t instrument) {
  return tracky->instruments[instrument].selected_pattern;
}

uint8_t tracker_instrument_selected_pattern_length(uint8_t instrument) {
  uint8_t selected_pattern = tracker_instrument_selected_pattern(instrument);
  return tracky->instruments[instrument].patterns[selected_pattern].length;
}

uint8_t tracker_instrument_selected_pattern_rhythm(uint8_t instrument) {
  uint8_t selected_pattern = tracker_instrument_selected_pattern(instrument);
  return tracky->instruments[instrument].patterns[selected_pattern].rhythm;
}

uint8_t *tracker_track_pattern_indeces(uint8_t track) {
  // Each instrument plays one pattern at a time.
  // The number of tracks, also the number of instruments.
  volatile uint8_t *patterns = tracky->tracks[track].patterns;
  uint8_t *indeces = calloc(NUMBER_OF_INSTRUMENTS * NOTES_PER_PATTERN, sizeof(uint8_t));
  for (uint8_t inst = 0; inst < NUMBER_OF_INSTRUMENTS; inst++) {
    struct instrument instr = tracky->instruments[inst];
    struct pattern p = instr.patterns[patterns[inst]];
    for (uint8_t index = 0; index < NOTES_PER_PATTERN; index++) {
      indeces[index * NUMBER_OF_INSTRUMENTS + inst] = p.notes[index].index;
    }
  }
  return indeces;
}

void tracker_selected_pattern_get(enum note_attribute attr, uint8_t *indeces) {
  for (uint8_t inst = 0; inst < NUMBER_OF_INSTRUMENTS; inst++) {
    struct instrument instr = tracky->instruments[inst];
    struct pattern p = instr.patterns[instr.selected_pattern];
    for (uint8_t index = 0; index < NOTES_PER_PATTERN; index++) {
      uint8_t attribute;
      if (attr == INDEX) {
        attribute = p.notes[index].index;
      } else if (attr == LENGTH) {
        attribute = p.notes[index].length;
      } else if (attr == ENVELOPE_STEP) {
        attribute = p.notes[index].envelope_step;
      } else if (attr == VOLUME) {
        attribute = p.notes[index].volume;
      } else {
        attribute = p.notes[index].index;
      }
      indeces[index*NUMBER_OF_INSTRUMENTS + inst] = attribute;
    }
  }
}

uint8_t tracker_selected_pattern_note_enabled(uint8_t instrument_number, uint8_t note_index) {
  uint8_t selected_pattern = tracky->instruments[instrument_number].selected_pattern;
  return tracky->instruments[instrument_number].patterns[selected_pattern].notes[note_index].enabled;
}

void tracker_selected_pattern_indeces(uint8_t *indeces) {
  tracker_selected_pattern_get(INDEX, indeces);
}

void tracker_selected_pattern_lengths(uint8_t *indeces) {
  tracker_selected_pattern_get(LENGTH, indeces);
}

void tracker_selected_pattern_envelope_steps(uint8_t *indeces) {
  tracker_selected_pattern_get(ENVELOPE_STEP, indeces);
}

void tracker_selected_pattern_volumes(uint8_t *indeces) {
  tracker_selected_pattern_get(VOLUME, indeces);
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

void tracker_copy_paste_notes(uint8_t instrument_src_index,
                              uint8_t pattern_src_index,
                              uint8_t note_src_index,
                              uint8_t instrument_dst_index,
                              uint8_t pattern_dst_index,
                              uint8_t note_dst_index,
                              uint8_t length) {
  for (uint8_t index = 0; index < length; index++) {
    tracky->instruments[instrument_dst_index].patterns[pattern_dst_index].notes[note_dst_index + index] =
      tracky->instruments[instrument_src_index].patterns[pattern_src_index].notes[note_src_index + index];
  }
}

void tracker_copy_paste_notes_attribute(uint8_t instrument_src_index,
                              uint8_t pattern_src_index,
                              uint8_t note_src_index,
                              uint8_t instrument_dst_index,
                              uint8_t pattern_dst_index,
                              uint8_t note_dst_index,
                              uint8_t length,
                              enum note_attribute attribute) {
  if (attribute == INDEX) {
    for (uint8_t index = 0; index < length; index++) {
      tracky->instruments[instrument_dst_index].patterns[pattern_dst_index].notes[note_dst_index + index].index =
        tracky->instruments[instrument_src_index].patterns[pattern_src_index].notes[note_src_index + index].index;
    }
  } else if (attribute == LENGTH) {
    for (uint8_t index = 0; index < length; index++) {
      tracky->instruments[instrument_dst_index].patterns[pattern_dst_index].notes[note_dst_index + index].length =
        tracky->instruments[instrument_src_index].patterns[pattern_src_index].notes[note_src_index + index].length;
    }
  } else if (attribute == ENVELOPE_STEP) {
    for (uint8_t index = 0; index < length; index++) {
      tracky->instruments[instrument_dst_index].patterns[pattern_dst_index].notes[note_dst_index + index].envelope_step =
        tracky->instruments[instrument_src_index].patterns[pattern_src_index].notes[note_src_index + index].envelope_step;
    }
  } else if (attribute == VOLUME) {
    for (uint8_t index = 0; index < length; index++) {
      tracky->instruments[instrument_dst_index].patterns[pattern_dst_index].notes[note_dst_index + index].volume =
        tracky->instruments[instrument_src_index].patterns[pattern_src_index].notes[note_src_index + index].volume;
    }
  }
}

struct note *get_note_of_instrument(uint8_t instrument_index, uint8_t step) {
  uint8_t selected_pattern = tracker_instrument_selected_pattern(instrument_index);
  uint8_t selected_pattern_length = tracker_instrument_selected_pattern_length(instrument_index);
  uint8_t pattern_step = step % selected_pattern_length;
  return  (struct note *) tracky->instruments[instrument_index].patterns[selected_pattern].notes + pattern_step;
}

void tracker_toggle_play_pause() {
  tracky->play_pause = ~tracky->play_pause;
}

void tracker_stop() {
  tracky->play_pause = 0;
}

uint8_t tracker_play_pause() {
  return tracky->play_pause;
}
