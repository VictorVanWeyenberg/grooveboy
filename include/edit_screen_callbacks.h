#ifndef EDIT_SCREEN_CALLBACKS_H_
#define EDIT_SCREEN_CALLBACKS_H_

#include <stdint.h>

extern uint8_t handle_page_flag;
extern uint8_t handle_clipboard_flag;

void edit_screen_null_function(uint8_t *args, uint8_t args_len);
void edit_note(uint8_t *args, uint8_t args_len);
void change_selected_pattern(uint8_t *args, uint8_t args_len);
void note_edit_mode(uint8_t *args, uint8_t args_len);
void hold_edit_mode(uint8_t *args, uint8_t args_len);
void decay_edit_mode(uint8_t *args, uint8_t args_len);
void amplitude_edit_mode(uint8_t *args, uint8_t args_len);
void set_instrument_pattern_length(uint8_t *args, uint8_t args_len);

#endif // EDIT_SCREEN_CALLBACKS_H_
