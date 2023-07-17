#include "edit_screen_callbacks.h"
#include "edit_screen.h"
#include "io.h"
#include "cursor.h"
#include "tracker.h"

extern uint8_t edit_screen_page;

component_callback *edit_screen_component_callbacks[3] = { edit_screen_null_function, edit_note, change_selected_pattern };

void edit_screen_null_function(uint8_t *args, uint8_t args_len) {
    move_cursor();
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
        if (key_pressed(KEY_UP)) {
            tracker_change_note(instrument, pattern, note_index, 12);
        }
        if (key_pressed(KEY_DOWN)) {
            tracker_change_note(instrument, pattern, note_index, -12);
        }
        if (key_pressed(KEY_LEFT)) {
            tracker_change_note(instrument, pattern, note_index, -1);
        }
        if (key_pressed(KEY_RIGHT)) {
            tracker_change_note(instrument, pattern, note_index, 1);
        }
        return;
    }
    // unselect_cursor();
    if (key_held(KEY_B)) {
        expand_cursor();
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
