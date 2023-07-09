#include "cursor.h"
#include "io.h"
#include <stdint.h>

extern struct component edit_screen_component_data;
extern short edit_screen_component_data_length;

struct component *components = &edit_screen_component_data;

uint8_t component_index = 0;

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

void move_cursor() {
    if (key_pressed(KEY_UP) && components[component_index].north != -1) {
        component_index = components[component_index].north;
    } else if (key_pressed(KEY_RIGHT) && components[component_index].east != -1) {
        component_index = components[component_index].east;
    } else if (key_pressed(KEY_DOWN) && components[component_index].south != -1) {
        component_index = components[component_index].south;
    } else if (key_pressed(KEY_LEFT) && components[component_index].west != -1) {
        component_index = components[component_index].west;
    } else {
        return;
    }
    struct component c = components[component_index];
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
