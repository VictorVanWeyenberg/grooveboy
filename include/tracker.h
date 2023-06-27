#ifndef __TRACKER_H
#define __TRACKER_H

#include <stdint.h>

#define NUMBER_OF_INSTRUMENTS 3
#define NUMBER_OF_TRACKS 16
#define PATTERNS_PER_INSTRUMENT 16
#define NOTES_PER_PATTERN 128

struct note {
  uint8_t index;
  uint16_t length: 6,
    duty: 2,
    envelope_step: 3,
    direction: 1,
    volume: 4;
} __attribute__((packed));

struct pattern {
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

uint8_t *tracker_track_pattern_indeces(uint8_t track);

uint8_t *tracker_track_pattern_lengths(uint8_t track);

#endif
