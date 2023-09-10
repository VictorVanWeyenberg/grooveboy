#ifndef SCREEN_COORDINATOR_H_
#define SCREEN_COORDINATOR_H_

#include "cursor.h"

typedef enum {
PLAY_SCREEN,
EDIT_SCREEN
} screen_type;

extern struct component *components;
extern uint8_t callbacks_length;
extern component_callback **callbacks;
extern uint8_t *args;
extern uint32_t all_args_len;

void set_screen_type(screen_type type);
void update_screen();
void update_screen_lock();
void update_screen_unlock();

#endif // SCREEN_COORDINATOR_H_
