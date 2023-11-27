#ifndef PLAY_SCREEN_CALLBACKS_H_
#define PLAY_SCREEN_CALLBACKS_H_

#include <stdint.h>

void play_screen_null_function(uint8_t *args, uint8_t args_len);
void set_pattern(uint8_t *args, uint8_t args_len);
void play(uint8_t *args, uint8_t args_len);
void pause(uint8_t *args, uint8_t args_len);
void stop(uint8_t *args, uint8_t args_len);
void edit(uint8_t *args, uint8_t args_len);
void save(uint8_t *args, uint8_t args_len);
void load(uint8_t *args, uint8_t args_len);

#endif // PLAY_SCREEN_CALLBACKS_H_
