#include <stdint.h>
#include "oam.h"

#ifndef CURSOR_H_
#define CURSOR_H_

struct component {
    uint8_t sx;
    uint8_t sy;
    uint8_t ex;
    uint8_t ey;
    int8_t north;
    int8_t east;
    int8_t south;
    int8_t west;
};

extern struct obj_attrs *cursor_nw;
extern struct obj_attrs *cursor_ne;
extern struct obj_attrs *cursor_se;
extern struct obj_attrs *cursor_sw;

void cursor_init();
void move_cursor();
void cursor_disable(uint8_t disable);

#endif // CURSOR_H_
