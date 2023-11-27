#include <stdlib.h>
#include "edit_screen_callbacks.h"
#include "edit_screen.h"
#include "io.h"
#include "cursor.h"
#include "tracker.h"
#include "screen_coordinator.h"

uint8_t edit_screen_component_callbacks_length = 9;
component_callback *volatile const edit_screen_component_callbacks[9] = {
    edit_screen_null_function,
    edit_note,
    change_selected_pattern,
    note_edit_mode,
    hold_edit_mode,
    decay_edit_mode,
    amplitude_edit_mode,
    set_instrument_pattern_length,
    play_screen
};
volatile uint8_t handle_page_flag = 1;
volatile uint8_t handle_clipboard_flag = 1;
volatile uint8_t clipboard_src_instrument = 0;
volatile uint8_t clipboard_src_pattern = 0;
volatile uint8_t clipboard_src_index = 0;
volatile uint8_t clipboard_length = 0;
enum clipboard_mode paste_mode = NOTES;

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
            update_screen_lock();
        } else if (key_pressed(KEY_UP) && edit_screen_page > 0) {
        refresh_cursor_position();
        edit_screen_page--;
        update_screen_lock();
    }
}

void toggle_clipboard_mode() {
    if (paste_mode == NOTES) {
        paste_mode = ATTRIBUTES;
    } else {
        paste_mode = NOTES;
    }
    update_screen_lock();
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
        update_screen_lock();
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
        update_screen_lock();
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
        update_screen_lock();
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
        update_screen_lock();
    }
    if (key_pressed(KEY_B)) {
        tracker_note_toggle_enable(instrument, pattern, note_index);
        update_screen_lock();
    }
}

void edit_note(uint8_t *args, uint8_t args_len) {
    if (args_len != 2) {
        return;
    }
    uint8_t instrument = args[0];
    uint8_t pattern = tracker_instrument_selected_pattern(instrument);
    uint8_t note_index = args[1] + 16 * edit_screen_page;
    if (key_pressed(KEY_A) && key_pressed(KEY_B)) {
        tracker_note_toggle_enable(instrument, pattern, note_index);
        update_screen_lock();
        return;
    }
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
            clipboard_src_pattern = tracker_instrument_selected_pattern(clipboard_src_instrument);
            clipboard_src_index = note_index;
            clipboard_length = cursor_size();
        }
        if (note_index < clipboard_src_index) {
            // In case cursor expand upwards.
            clipboard_src_index = note_index;
        }
    } else if (key_held(KEY_L)) {
        if (key_pressed(KEY_A)) {
            if (paste_mode == NOTES) {
                uint8_t pattern = tracker_instrument_selected_pattern(instrument);
                tracker_copy_paste_notes(clipboard_src_instrument, clipboard_src_pattern, clipboard_src_index,
                                        instrument, pattern, note_index, clipboard_length);
                update_screen_lock();
            } else if (paste_mode == ATTRIBUTES) {
                uint8_t pattern = tracker_instrument_selected_pattern(instrument);
                enum note_attribute attribute = INDEX;
                if (mode == NOTE) {
                    attribute = INDEX;
                } else if (mode == HOLD) {
                    attribute = LENGTH;
                } else if (mode == DECAY) {
                    attribute = ENVELOPE_STEP;
                } else if (mode == AMPLITUDE) {
                    attribute = VOLUME;
                }
                tracker_copy_paste_notes_attribute(clipboard_src_instrument, clipboard_src_pattern, clipboard_src_index,
                                        instrument, pattern, note_index, clipboard_length, attribute);
                update_screen_lock();
            }
        }
        if (key_pressed(KEY_B)) {
            toggle_clipboard_mode();
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
            update_screen_lock();
        }
        if (key_pressed(KEY_LEFT) || key_pressed(KEY_DOWN)) {
            tracker_change_selected_pattern(instrument_number, -1);
            update_screen_lock();
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
        update_screen_lock();
    }
    move_cursor();
}

void hold_edit_mode(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) {
        return;
    }
    if (key_pressed(KEY_A)) {
        set_edit_mode(HOLD);
        update_screen_lock();
    }
    move_cursor();
}

void decay_edit_mode(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) {
        return;
    }
    if (key_pressed(KEY_A)) {
        set_edit_mode(DECAY);
        update_screen_lock();
    }
    move_cursor();
}

void amplitude_edit_mode(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) {
        return;
    }
    if (key_pressed(KEY_A)) {
        set_edit_mode(AMPLITUDE);
        update_screen_lock();
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
            update_screen_lock();
        }
        if (key_pressed(KEY_DOWN)) {
            tracker_change_selected_pattern_length(instrument_index, -16);
            update_screen_lock();
        }
        if (key_pressed(KEY_RIGHT) || key_held(KEY_RIGHT)) {
            tracker_change_selected_pattern_length(instrument_index, 1);
            update_screen_lock();
        }
        if (key_pressed(KEY_LEFT) || key_held(KEY_LEFT)) {
            tracker_change_selected_pattern_length(instrument_index, -1);
            update_screen_lock();
        }
        return;
    }
    move_cursor();
}

void play_screen(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) {
        return;
    }
    if (key_pressed(KEY_A)) {
        set_screen_type(PLAY_SCREEN);
        return;
    }
    move_cursor();
}
