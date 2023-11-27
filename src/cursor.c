#include "cursor.h"
#include "screen_coordinator.h"
#include "io.h"
#include <stdio.h>
#include <stdint.h>
#include "kdq.h"
KDQ_INIT(uint8_t)

#define min(a,b) a > b ? b : a
#define max(a,b) a > b ? a : b

volatile struct obj_attrs *cursor_nw;
volatile struct obj_attrs *cursor_ne;
volatile struct obj_attrs *cursor_se;
volatile struct obj_attrs *cursor_sw;

kdq_t(uint8_t) *volatile component_queue;
volatile uint8_t handle_cursor_flag = 1;

void cursor_init() {
    component_queue = kdq_init(uint8_t);
    uint8_t ci = 0;
    while (components[ci].north != -1) ci = components[ci].north;
    while (components[ci].west != -1) ci = components[ci].west;
    kdq_push(uint8_t, component_queue, ci);
    cursor_nw = object_create(0, 0, 0, 0, 1);
    cursor_ne = object_create(0, 0, 1, 0, 1);
    cursor_se = object_create(0, 0, 1, 1, 1);
    cursor_sw = object_create(0, 0, 0, 1, 1);
    refresh_cursor_position();
}

void expand_cursor() {
    if (handle_cursor_flag == 0) {
        return;
    }
    handle_cursor_flag = 0;
    int8_t north_index = components[kdq_last(component_queue)].north;
    int8_t south_index = components[kdq_last(component_queue)].south;
    if ((key_pressed(KEY_UP) || key_held(KEY_UP)) && north_index != -1 &&
        components[kdq_first(component_queue)].sx == components[north_index].sx &&
        components[kdq_first(component_queue)].callback_index == components[north_index].callback_index) {
        uint8_t component_queue_size = kdq_size(component_queue);
        if (component_queue_size > 1 && kdq_at(component_queue, component_queue_size - 2) == north_index) {
            kdq_pop(uint8_t, component_queue);
        } else {
            kdq_push(uint8_t, component_queue, north_index);
        }
    } else if ((key_pressed(KEY_DOWN) || key_held(KEY_DOWN)) && south_index != -1 &&
               components[kdq_first(component_queue)].sx == components[south_index].sx &&
               components[kdq_first(component_queue)].callback_index == components[south_index].callback_index) {
        uint8_t component_queue_size = kdq_size(component_queue);
        if (component_queue_size > 1 && kdq_at(component_queue, component_queue_size - 2) == south_index) {
            kdq_pop(uint8_t, component_queue);
        } else {
            kdq_push(uint8_t, component_queue, south_index);
        }
    } else {
        return;
    }
    struct component bc = components[kdq_first(component_queue)];
    struct component ec = components[kdq_last(component_queue)];
    set_cursor_position(min(bc.sx,ec.sx), min(bc.sy,ec.sy), max(bc.ex,ec.ex), max(bc.ey,ec.ey));
}

void move_cursor() {
    if (handle_cursor_flag == 0) {
        return;
    }
    handle_cursor_flag = 0;
    uint8_t first = kdq_last(component_queue);
    if ((key_pressed(KEY_UP) || key_held(KEY_UP)) && components[first].north != -1) {
        while (kdq_size(component_queue) > 1) kdq_pop(uint8_t, component_queue);
        kdq_pop(uint8_t, component_queue);
        kdq_push(uint8_t, component_queue, components[first].north);
    } else if ((key_pressed(KEY_RIGHT) || key_held(KEY_RIGHT)) && components[first].east != -1) {
        while (kdq_size(component_queue) > 1) kdq_pop(uint8_t, component_queue);
        kdq_pop(uint8_t, component_queue);
        kdq_push(uint8_t, component_queue, components[first].east);
    } else if ((key_pressed(KEY_DOWN) || key_held(KEY_DOWN)) && components[first].south != -1) {
        while (kdq_size(component_queue) > 1) kdq_pop(uint8_t, component_queue);
        kdq_pop(uint8_t, component_queue);
        kdq_push(uint8_t, component_queue, components[first].south);
    } else if ((key_pressed(KEY_LEFT) || key_held(KEY_LEFT)) && components[first].west != -1) {
        while (kdq_size(component_queue) > 1) kdq_pop(uint8_t, component_queue);
        kdq_pop(uint8_t, component_queue);
        kdq_push(uint8_t, component_queue, components[first].west);
    } else {
        return;
    }
    refresh_cursor_position();
}

void reset_cursor_position() {
    uint8_t ci = 0;
    while (components[ci].north != -1) ci = components[ci].north;
    while (components[ci].west != -1) ci = components[ci].west;
    kdq_push(uint8_t, component_queue, ci);
    struct component c = components[ci];
    set_cursor_position(c.sx, c.sy, c.ex, c.ey);
}

void refresh_cursor_position() {
    while (kdq_size(component_queue) > 1) kdq_pop(uint8_t, component_queue);
    struct component c = components[kdq_first(component_queue)];
    set_cursor_position(c.sx, c.sy, c.ex, c.ey);
}

void set_cursor_position(uint8_t sx, uint8_t sy, uint8_t ex, uint8_t ey) {
    cursor_nw->x = sx - 1;
    cursor_nw->y = sy - 1;
    cursor_ne->x = ex + 1;
    cursor_ne->y = sy - 1;
    cursor_se->x = ex + 1;
    cursor_se->y = ey + 1;
    cursor_sw->x = sx - 1;
    cursor_sw->y = ey + 1;
}

void cursor_disable(uint8_t disable) {
    cursor_nw->disable = disable & 1;
    cursor_ne->disable = disable & 1;
    cursor_se->disable = disable & 1;
    cursor_sw->disable = disable & 1;
}

uint8_t cursor_size() {
    return kdq_size(component_queue);
}

void cursor_component_method() {
    for (uint8_t iter = 0; iter < kdq_size(component_queue); iter++) {
        uint8_t index = kdq_at(component_queue, iter);
        uint8_t callback_index = components[index].callback_index;
        if (callback_index > callbacks_length) {
            continue;
        }
        uint8_t args_index = components[index].args_index;
        uint8_t args_len = components[index].args_len;
        uint8_t *component_args = &args[0];
        if (args_index < all_args_len) {
            component_args = &args[args_index];
        }
        callbacks[callback_index](component_args, args_len);
    }
    handle_cursor_flag = 1;
}
