#ifndef SCREEN_COORDINATOR_H_
#define SCREEN_COORDINATOR_H_

#include "cursor.h"

typedef enum {
PLAY_SCREEN,
EDIT_SCREEN
} screen_type;

extern struct component *components;
extern component_callback **callbacks;
extern uint8_t *args;
extern uint32_t all_args_len;

void set_screen_type(screen_type type);

#endif // SCREEN_COORDINATOR_H_
