#ifndef EDIT_SCREEN_CALLBACKS_H_
#define EDIT_SCREEN_CALLBACKS_H_

#include <stdint.h>

void edit_screen_null_function(uint8_t *args, uint8_t args_len);
void edit_note(uint8_t *args, uint8_t args_len);
void change_selected_pattern(uint8_t *args, uint8_t args_len);

#endif // EDIT_SCREEN_CALLBACKS_H_
