#include <stdlib.h>
#include "edit_screen_callbacks.h"
#include "edit_screen.h"
#include "io.h"
#include "cursor.h"
#include "tracker.h"

component_callback *edit_screen_component_callbacks[8] = {
    edit_screen_null_function,
    edit_note,
    change_selected_pattern,
    note_edit_mode,
    hold_edit_mode,
    decay_edit_mode,
    amplitude_edit_mode,
    set_instrument_pattern_length
};
uint8_t handle_page_flag = 1;
uint8_t handle_clipboard_flag = 1;
uint8_t clipboard_src_instrument = -1;
uint8_t clipboard_src_pattern = -1;
uint8_t clipboard_src_index = -1;
uint8_t clipboard_length = 0;

void edit_screen_null_function(uint8_t *args, uint8_t args_len) {
    move_cursor();
}

void change_page() {
    if (handle_page_flag == 0) {
        return;
    }
    handle_page_flag = 0;
    if (key_pressed(KEY_DOWN) && edit_screen_page < (NOTES_PER_PATTERN / 16) - 1) {
        refresh_cursor_position();
        edit_screen_page++;
    } else if (key_pressed(KEY_UP) && edit_screen_page > 0) {
        refresh_cursor_position();
        edit_screen_page--;
    }
}

void change_tracker_attributes(uint8_t instrument, uint8_t pattern, uint8_t note_index) {
    if (key_pressed(KEY_UP)) {
        if (mode == NOTE) {
            tracker_change_note(instrument, pattern, note_index, 12);
        } else if (mode == HOLD) {
            tracker_change_length(instrument, pattern, note_index, 5);
        } else if (mode == DECAY) {
            tracker_change_envelope_duty(instrument, pattern, note_index, 1);
        } else if (mode == AMPLITUDE) {
            tracker_change_volume(instrument, pattern, note_index, 1);
        }
    }
    if (key_pressed(KEY_DOWN)) {
        if (mode == NOTE) {
            tracker_change_note(instrument, pattern, note_index, -12);
        } else if (mode == HOLD) {
            tracker_change_length(instrument, pattern, note_index, -5);
        } else if (mode == DECAY) {
            tracker_change_envelope_duty(instrument, pattern, note_index, -1);
        } else if (mode == AMPLITUDE) {
            tracker_change_volume(instrument, pattern, note_index, -1);
        }
    }
    if (key_pressed(KEY_LEFT) || key_held(KEY_LEFT)) {
        if (mode == NOTE) {
            tracker_change_note(instrument, pattern, note_index, -1);
        } else if (mode == HOLD) {
            tracker_change_length(instrument, pattern, note_index, -1);
        } else if (mode == DECAY) {
            tracker_change_envelope_duty(instrument, pattern, note_index, -1);
        } else if (mode == AMPLITUDE) {
            tracker_change_volume(instrument, pattern, note_index, -1);
        }
    }
    if (key_pressed(KEY_RIGHT) || key_held(KEY_RIGHT)) {
        if (mode == NOTE) {
            tracker_change_note(instrument, pattern, note_index, 1);
        } else if (mode == HOLD) {
            tracker_change_length(instrument, pattern, note_index, 1);
        } else if (mode == DECAY) {
            tracker_change_envelope_duty(instrument, pattern, note_index, 1);
        } else if (mode == AMPLITUDE) {
            tracker_change_volume(instrument, pattern, note_index, 1);
        }
    }
}

void edit_note(uint8_t *args, uint8_t args_len) {
    if (args_len != 2) {
        return;
    }
    uint8_t instrument = args[0];
    uint8_t pattern = tracker_instrument_selected_pattern(instrument);
    uint8_t note_index = args[1] + 16 * edit_screen_page;
    if (key_held(KEY_A)) {
        // select_cursor();
        change_tracker_attributes(instrument, pattern, note_index);
        return;
    }
    // unselect_cursor();
    if (key_held(KEY_B)) {
        expand_cursor();
    } else if (key_held(KEY_R)) {
        change_page();
    } else if (key_pressed(KEY_L)) {
        if (handle_clipboard_flag == 1) {
            handle_clipboard_flag = 0;
            clipboard_src_instrument = instrument;
            clipboard_src_pattern = tracky->instruments[clipboard_src_instrument].selected_pattern;
            clipboard_src_index = note_index;
            clipboard_length = cursor_size();
        }
        if (note_index < clipboard_src_index) {
            // In case cursor expand upwards.
            clipboard_src_index = note_index;
        }
    } else if (key_held(KEY_L)) {
        if (key_pressed(KEY_A))  {
            uint8_t pattern = tracky->instruments[instrument].selected_pattern;
            tracker_copy_paste_notes(clipboard_src_instrument, clipboard_src_pattern, clipboard_src_index,
                                     instrument, pattern, note_index, clipboard_length);
        }
    } else {
        move_cursor();
    }
}

void change_selected_pattern(uint8_t *args, uint8_t args_len) {
    if (args_len != 1) {
        return;
    }
    uint8_t instrument_number = args[0];
    if (key_held(KEY_A)) {
        // select_cursor();
        if (key_pressed(KEY_UP) || key_pressed(KEY_RIGHT)) {
            tracker_change_selected_pattern(instrument_number, 1);
        }
        if (key_pressed(KEY_LEFT) || key_pressed(KEY_DOWN)) {
            tracker_change_selected_pattern(instrument_number, -1);
        }
        return;
    }
    // unselect_cursor();
    move_cursor();
}

void note_edit_mode(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) {
        return;
    }
    if (key_pressed(KEY_A)) {
        set_edit_mode(NOTE);
    }
    move_cursor();
}

void hold_edit_mode(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) {
        return;
    }
    if (key_pressed(KEY_A)) {
        set_edit_mode(HOLD);
    }
    move_cursor();
}

void decay_edit_mode(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) {
        return;
    }
    if (key_pressed(KEY_A)) {
        set_edit_mode(DECAY);
    }
    move_cursor();
}

void amplitude_edit_mode(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) {
        return;
    }
    if (key_pressed(KEY_A)) {
        set_edit_mode(AMPLITUDE);
    }
    move_cursor();
}

void set_instrument_pattern_length(uint8_t *args, uint8_t args_len) {
    if (args_len != 1) {
        return;
    }
    uint8_t instrument_index = args[0];
    if (key_held(KEY_A)) {
        if (key_pressed(KEY_UP)) {
            tracker_change_selected_pattern_length(instrument_index, 16);
        }
        if (key_pressed(KEY_DOWN)) {
            tracker_change_selected_pattern_length(instrument_index, -16);
        }
        if (key_pressed(KEY_RIGHT) || key_held(KEY_RIGHT)) {
            tracker_change_selected_pattern_length(instrument_index, 1);
        }
        if (key_pressed(KEY_LEFT) || key_held(KEY_LEFT)) {
            tracker_change_selected_pattern_length(instrument_index, -1);
        }
        return;
    }
    move_cursor();
}
