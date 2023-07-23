#ifndef __EDIT_SCREEN
#define __EDIT_SCREEN

#include <stdint.h>

enum edit_mode {
NOTE,HOLD,DECAY,AMPLITUDE
};

extern uint8_t edit_screen_page;
extern enum edit_mode mode;

void edit_screen_init();
void update_edit_screen_notes();
void set_edit_mode(enum edit_mode set_mode);

#endif
