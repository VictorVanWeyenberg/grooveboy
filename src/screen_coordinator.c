#include "screen_coordinator.h"
#include "cursor.h"
#include "dma.h"
#include "vram.h"

extern short edit_screen_BG00_screen_data;
extern short edit_screen_BG00_screen_data_length;
extern short edit_screen_BG01_screen_data;
extern short edit_screen_BG01_screen_data_length;
extern short edit_screen_BG02_screen_data;
extern short edit_screen_BG02_screen_data_length;

extern struct component edit_screen_component_data;
extern short edit_screen_component_data_length;
extern uint8_t edit_screen_component_args;
extern int edit_screen_component_args_length;
extern component_callback *edit_screen_component_callbacks[];

struct component *components;
component_callback **callbacks;
uint8_t *args;
uint32_t all_args_len;

void set_screen_type(screen_type type) {
    if (type == EDIT_SCREEN) {
        dma_push(1, &edit_screen_BG00_screen_data, edit_screen_BG00_screen_data_length, MEM_BG0_SCREEN_BLOCK);
        dma_push(1, &edit_screen_BG01_screen_data, edit_screen_BG01_screen_data_length, MEM_BG1_SCREEN_BLOCK);
        dma_push(1, &edit_screen_BG02_screen_data, edit_screen_BG02_screen_data_length, MEM_BG2_SCREEN_BLOCK);
        dma_on(1);
        components = &edit_screen_component_data;
        callbacks = edit_screen_component_callbacks;
        args = &edit_screen_component_args;
        all_args_len = edit_screen_component_args_length;
    }
}
