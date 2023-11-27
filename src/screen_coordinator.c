#include "screen_coordinator.h"
#include "cursor.h"
#include "dma.h"
#include "vram.h"
#include "edit_screen.h"
#include "string.h"

extern short edit_screen_BG00_screen_data;
extern short edit_screen_BG00_screen_data_length;
extern short edit_screen_BG01_screen_data;
extern short edit_screen_BG01_screen_data_length;
extern short edit_screen_BG02_screen_data;
extern short edit_screen_BG02_screen_data_length;

extern short play_screen_BG00_screen_data;
extern short play_screen_BG00_screen_data_length;
extern short play_screen_BG01_screen_data;
extern short play_screen_BG01_screen_data_length;
extern short play_screen_BG02_screen_data;
extern short play_screen_BG02_screen_data_length;

extern struct component edit_screen_component_data;
extern short edit_screen_component_data_length;
extern uint8_t edit_screen_component_args;
extern int edit_screen_component_args_length;
extern component_callback *edit_screen_component_callbacks[];
extern uint8_t edit_screen_component_callbacks_length;

extern struct component play_screen_component_data;
extern short play_screen_component_data_length;
extern uint8_t play_screen_component_args;
extern int play_screen_component_args_length;
extern component_callback *play_screen_component_callbacks[];
extern uint8_t play_screen_component_callbacks_length;

struct component *components;
component_callback **callbacks;
uint8_t callbacks_length;
uint8_t *args;
uint32_t all_args_len;
volatile screen_type current_type = EDIT_SCREEN;
volatile uint8_t update_screen_flag = 0;

void set_screen_type(screen_type type) {
    current_type = type;
    if (type == PLAY_SCREEN) {
        memcpy(MEM_BG0_SCREEN_BLOCK, &play_screen_BG00_screen_data, play_screen_BG00_screen_data_length);
        memcpy(MEM_BG1_SCREEN_BLOCK, &play_screen_BG01_screen_data, play_screen_BG01_screen_data_length);
        memcpy(MEM_BG2_SCREEN_BLOCK, &play_screen_BG02_screen_data, play_screen_BG02_screen_data_length);
        components = &play_screen_component_data;
        callbacks = play_screen_component_callbacks;
        callbacks_length = play_screen_component_callbacks_length;
        args = &play_screen_component_args;
        all_args_len = play_screen_component_args_length;
    } else if (type == EDIT_SCREEN) {
        memcpy(MEM_BG0_SCREEN_BLOCK, &edit_screen_BG00_screen_data, edit_screen_BG00_screen_data_length);
        memcpy(MEM_BG1_SCREEN_BLOCK, &edit_screen_BG01_screen_data, edit_screen_BG01_screen_data_length);
        memcpy(MEM_BG2_SCREEN_BLOCK, &edit_screen_BG02_screen_data, edit_screen_BG02_screen_data_length);
        components = &edit_screen_component_data;
        callbacks = edit_screen_component_callbacks;
        callbacks_length = edit_screen_component_callbacks_length;
        args = &edit_screen_component_args;
        all_args_len = edit_screen_component_args_length;
    }
    reset_cursor_position();
}

void update_screen_lock() {
        update_screen_flag = 0;
}

void update_screen_unlock() {
        update_screen_flag = 1;
}

void update_screen() {
        if (update_screen_flag) return;
        update_screen_unlock();
        if (current_type == EDIT_SCREEN) {
                update_edit_screen_notes();
        }
}
