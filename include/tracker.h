#ifndef __TRACKER_H
#define __TRACKER_H

#include <stdint.h>

#define NUMBER_OF_INSTRUMENTS 4
#define NUMBER_OF_TRACKS 16
#define PATTERNS_PER_INSTRUMENT 16
#define NOTES_PER_PATTERN 128

enum note_attribute {
INDEX, LENGTH, ENVELOPE_STEP, VOLUME
};

struct note {
  uint8_t index;
  uint16_t length: 6,
    duty: 2,
    envelope_step: 3,
    direction: 1,
    volume: 4;
} __attribute__((packed));

struct pattern {
  uint8_t length; // Read: Last step
  struct note notes[NOTES_PER_PATTERN];
};

struct instrument {
  uint8_t selected_pattern;
  struct pattern patterns[PATTERNS_PER_INSTRUMENT];
} __attribute__((packed));

struct track {
  uint8_t patterns[NUMBER_OF_INSTRUMENTS];
} __attribute__((packed));

struct tracker {
  struct instrument instruments[NUMBER_OF_INSTRUMENTS];
  struct track tracks[NUMBER_OF_TRACKS];
} __attribute__((packed));

extern struct tracker *tracky;

void tracker_create();

void tracker_change_note(uint8_t instrument, uint8_t pattern, uint8_t note_index, int8_t offset);
void tracker_change_length(uint8_t instrument, uint8_t pattern, uint8_t note_index, int8_t offset);
void tracker_change_envelope_duty(uint8_t instrument, uint8_t pattern, uint8_t note_index, int8_t offset);
void tracker_change_volume(uint8_t instrument, uint8_t pattern, uint8_t note_index, int8_t offset);

uint8_t tracker_instrument_selected_pattern(uint8_t instrument);

uint8_t *tracker_track_pattern_indeces(uint8_t track);

uint8_t *tracker_selected_pattern_indeces();
uint8_t *tracker_selected_pattern_lengths();
uint8_t *tracker_selected_pattern_envelope_steps();
uint8_t *tracker_selected_pattern_volumes();
void tracker_change_selected_pattern_length(uint8_t instrument_index, int8_t offset);

void tracker_change_selected_pattern(uint8_t instrument_number, int8_t offset);

uint8_t *tracker_track_pattern_lengths(uint8_t track);

void tracker_copy_paste_notes(uint8_t instrument_src_index,
                              uint8_t pattern_src_index,
                              uint8_t note_src_index,
                              uint8_t instrument_dst_index,
                              uint8_t pattern_dst_index,
                              uint8_t note_dst_index,
                              uint8_t length);
void tracker_copy_paste_notes_attribute(uint8_t instrument_src_index,
                              uint8_t pattern_src_index,
                              uint8_t note_src_index,
                              uint8_t instrument_dst_index,
                              uint8_t pattern_dst_index,
                              uint8_t note_dst_index,
                              uint8_t length,
                              enum note_attribute attribute);

#endif
