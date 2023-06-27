#include <stdio.h>
#include "tracker.h"
#include "minunit/minunit.h"

MU_TEST(tracker_notes_index_initialization) {
  tracker_create();
  for (uint8_t n_instr = 0; n_instr < NUMBER_OF_INSTRUMENTS; n_instr++) {
    struct instrument instr = tracky->instruments[n_instr];
    for (uint8_t n_patt = 0; n_patt < PATTERNS_PER_INSTRUMENT; n_patt++) {
      struct pattern patt = instr.patterns[n_patt];
      for (uint8_t index = 0; index < NOTES_PER_PATTERN; index++) {
	mu_check(36 == patt.notes[index].index);
      }
    }
  }
}

MU_TEST(tracker_track_pattern_indeces_test) {
  tracker_create();
  uint8_t *indeces = tracker_track_pattern_indeces(0);
  for (uint16_t i = 0; i < NOTES_PER_PATTERN * 3; i++) {
    mu_check(indeces[i] == 36);
  }
}

MU_TEST(tracker_track_pattern_lengths_test) {
  tracker_create();
  uint8_t *lengths = tracker_track_pattern_lengths(0);
  for (uint16_t i = 0; i < NOTES_PER_PATTERN * 3; i++) {
    mu_check(lengths[i] == 32);
  }
}

MU_TEST(note_size) {
  printf("%ld\n", sizeof(struct note));
  mu_check(sizeof(struct note) == 3);
}

MU_TEST_SUITE(test_suite) {
  MU_RUN_TEST(tracker_notes_index_initialization);
  MU_RUN_TEST(tracker_track_pattern_indeces_test);
  MU_RUN_TEST(tracker_track_pattern_lengths_test);
  MU_RUN_TEST(note_size);
}

int main(void) {
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  return MU_EXIT_CODE;
}
