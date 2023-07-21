#include <stdlib.h>
#include "edit_screen_callbacks.h"
#include "edit_screen.h"
#include "io.h"
#include "cursor.h"
#include "tracker.h"

component_callback *edit_screen_component_callbacks[3] = { edit_screen_null_function, edit_note, change_selected_pattern };
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
    if (key_pressed(KEY_DOWN) && edit_screen_page < 3) {
        refresh_cursor_position();
        edit_screen_page++;
    } else if (key_pressed(KEY_UP) && edit_screen_page > 0) {
        refresh_cursor_position();
        edit_screen_page--;
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
