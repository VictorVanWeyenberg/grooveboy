#include "io.h"
#include "stdlib.h"
#include "string.h"
#include "tracker.h"

uint8_t N_BUTTONS = 10;
uint16_t *states;
uint8_t locked = 0;

void init_io() {
    states = calloc(N_BUTTONS, sizeof(uint8_t));
}

void register_presses() {
    if (locked == 1) return;
    uint16_t state = ~*REG_KEY_INPUT;
    for (uint8_t key = 0; key < N_BUTTONS; key++) {
        states[key] = (states[key] << 1) | ((state & (1 << key)) >> key);
    }
}

uint16_t key_pressed(uint16_t key) {
    return ((states[key] & 0x81) == 0) && ((states[key] & 0x7E) != 0);
}

uint16_t key_held(uint16_t key) {
    return states[key] >= 0xff;
}

void loop_end() {
    if (locked == 1) return;
    for (uint8_t key = 0; key < N_BUTTONS; key++) {
        if (key_pressed(key)) {
            states[key] = 0x00;
        }
    }
}

void lock_io() {
    locked = 1;
}

void unlock_io() {
    locked = 0;
}
