#include "play_screen_callbacks.h"
#include "screen_coordinator.h"
#include "cursor.h"
#include "io.h"

uint8_t play_screen_component_callbacks_length = 8;
component_callback *volatile const play_screen_component_callbacks[8] = {
    play_screen_null_function,
    set_pattern,
    play,
    pause,
    stop,
    edit,
    save,
    load
};

void play_screen_null_function(uint8_t *args, uint8_t args_len) {
    move_cursor();
}

void set_pattern(uint8_t *args, uint8_t args_len) {
    if (args_len != 2) return;
    move_cursor();
}

void play(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) return;
    move_cursor();
}

void pause(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) return;
    move_cursor();
}

void stop(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) return;
    move_cursor();
}

void edit(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) return;
    if (key_pressed(KEY_A)) {
        set_screen_type(EDIT_SCREEN);
        return;
    }
    move_cursor();
}

void save(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) return;
    move_cursor();
}

void load(uint8_t *args, uint8_t args_len) {
    if (args_len != 0) return;
    move_cursor();
}
