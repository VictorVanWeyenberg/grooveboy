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

extern struct obj_attrs *cursor_nw;
extern struct obj_attrs *cursor_ne;
extern struct obj_attrs *cursor_se;
extern struct obj_attrs *cursor_sw;

void cursor_init();
void move_cursor();
void expand_cursor();
void cursor_disable(uint8_t disable);
void cursor_component_method();

#endif // CURSOR_H_
