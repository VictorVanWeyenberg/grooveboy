#include "io.h"
#include "stdlib.h"
#include "string.h"

uint16_t *states;
uint8_t state_changed = 0;

void init_io() {
    states = malloc(sizeof(uint16_t) * 3);
}

void loop_end() {
    state_changed = 0;
}

void register_presses() {
    states[2] = states[1];
    states[1] = states[0];
    memcpy(states, (void *) 0x4000130, 2);
    state_changed = states[0] != states[1];
}

uint16_t key_pressed(uint16_t key) {
    return state_changed && ((states[2] & key) > 0) && ((states[1] & key) == 0) && ((states[0] & key) > 0);
}

uint16_t key_held(uint16_t key) {
    return ((states[2] & key) == 0) && ((states[1] & key) == 0) && ((states[0] & key) == 0);
}
