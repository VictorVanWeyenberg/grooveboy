#include "cursor.h"
#include "screen_coordinator.h"
#include "io.h"
#include <stdint.h>

#define min(a,b) a > b ? b : a
#define max(a,b) a > b ? a : b

uint8_t component_begin_index = 0;
uint8_t component_end_index = 0;

struct obj_attrs *cursor_nw;
struct obj_attrs *cursor_ne;
struct obj_attrs *cursor_se;
struct obj_attrs *cursor_sw;

void cursor_init() {
    cursor_nw = object_create(0, 0, 0, 0, 1);
    cursor_ne = object_create(0, 0, 1, 0, 1);
    cursor_se = object_create(0, 0, 1, 1, 1);
    cursor_sw = object_create(0, 0, 0, 1, 1);
    move_cursor();
}

void expand_cursor() {
    if (key_pressed(KEY_UP) && components[component_end_index].north != -1 &&
        components[component_begin_index].sx == components[components[component_end_index].north].sx &&
        components[component_begin_index].callback_index == components[components[component_end_index].north].callback_index) {
        component_end_index = components[component_end_index].north;
    } else if (key_pressed(KEY_DOWN) && components[component_begin_index].south != -1 &&
               components[component_begin_index].sx == components[components[component_end_index].south].sx &&
               components[component_begin_index].callback_index == components[components[component_end_index].south].callback_index) {
        component_end_index = components[component_end_index].south;
    } else {
        return;
    }
    struct component bc = components[component_begin_index];
    struct component ec = components[component_end_index];
    cursor_nw->x = min(bc.sx, ec.sx);
    cursor_nw->y = min(bc.sy, ec.sy);
    cursor_ne->x = max(bc.ex, ec.ex);
    cursor_ne->y = min(bc.sy, ec.sy);
    cursor_se->x = max(bc.ex, ec.ex);
    cursor_se->y = max(bc.ey, ec.ey);
    cursor_sw->x = min(bc.sx, ec.sx);
    cursor_sw->y = max(bc.ey, ec.ey);
}

void move_cursor() {
    if (key_pressed(KEY_UP) && components[component_begin_index].north != -1) {
        component_begin_index = components[component_begin_index].north;
        component_end_index = component_begin_index;
    } else if (key_pressed(KEY_RIGHT) && components[component_begin_index].east != -1) {
        component_begin_index = components[component_begin_index].east;
        component_end_index = component_begin_index;
    } else if (key_pressed(KEY_DOWN) && components[component_begin_index].south != -1) {
        component_begin_index = components[component_begin_index].south;
        component_end_index = component_begin_index;
    } else if (key_pressed(KEY_LEFT) && components[component_begin_index].west != -1) {
        component_begin_index = components[component_begin_index].west;
        component_end_index = component_begin_index;
    } else {
        return;
    }
    struct component c = components[component_begin_index];
    cursor_nw->x = c.sx;
    cursor_nw->y = c.sy;
    cursor_ne->x = c.ex;
    cursor_ne->y = c.sy;
    cursor_se->x = c.ex;
    cursor_se->y = c.ey;
    cursor_sw->x = c.sx;
    cursor_sw->y = c.ey;
}

void cursor_disable(uint8_t disable) {
    cursor_nw->disable = disable & 1;
    cursor_ne->disable = disable & 1;
    cursor_se->disable = disable & 1;
    cursor_sw->disable = disable & 1;
}

void cursor_component_method() {
    uint8_t callback_index = components[component_begin_index].callback_index;
    uint8_t args_index = components[component_begin_index].args_index;
    uint8_t args_len = components[component_begin_index].args_len;
    uint8_t *component_args = &args[0];
    if (args_index < all_args_len) {
        component_args = &args[args_index];
    }
    callbacks[callback_index](component_args, args_len);
}
