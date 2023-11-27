#include <stdint.h>
#include "oam.h"

#ifndef CURSOR_H_
#define CURSOR_H_

typedef void component_callback(uint8_t *args, uint8_t args_len);

struct component {
    uint8_t sx;
    uint8_t sy;
    uint8_t ex;
    uint8_t ey;
    int8_t north;
    int8_t east;
    int8_t south;
    int8_t west;
    uint8_t callback_index;
    uint8_t args_index;
    uint8_t args_len;
};

void cursor_init();
void move_cursor();
void expand_cursor();
void reset_cursor_position();
void refresh_cursor_position();
void set_cursor_position(uint8_t sx, uint8_t sy, uint8_t ex, uint8_t ey);
void cursor_disable(uint8_t disable);
uint8_t cursor_size();
void cursor_component_method();

#endif // CURSOR_H_
