#include "snd.h"
#include "tracker.h"
#include <math.h>

uint8_t step = 0;

uint32_t midi_key_to_rate(uint8_t key) {
    double octave = (key - 69.0) / 12.0;
    double freq = 440 * pow(2, octave);
    return round((2048 * freq - 131072) / freq);
}

uint16_t midi_key_to_sample_rate(uint8_t key) {
    double freq = 440 * pow(2, (key - 69) / 12);
    return (uint16_t) round((2048 - freq) / 2097152);
}

void trigger_sound() {
    step %= NOTES_PER_PATTERN;

    volatile struct note *note0 = get_note_of_instrument(0, step);
    REG_SOUND1CNT_L = 0;
    REG_SOUND1CNT_H = (note0->length & 0x1F) | 0x80 | ((note0->envelope_step & 0x7) << 8) | ((note0->volume & 0xF) << 12);
    REG_SOUND1CNT_X = midi_key_to_rate(note0->index);

    /* REG_SOUND2CNT_L = notes[1]->length |
        0x80 |
        (notes[1]->envelope_step << 8) |
        (notes[1]->volume << 12);
    REG_SOUND2CNT_H = midi_key_to_freq(notes[1]->index); */

    /* REG_SOUND3CNT_L = SOUND3BANK64 |
        SOUND3SETBANK0 |
        SOUND3PLAY;
    REG_SOUND3CNT_H = notes[2]->length |
        SOUND3OUTPUT1;
    REG_SOUND3CNT_X = midi_key_to_sample_rate(notes[2]->index) |
        SOUND3PLAYONCE; */

    /* REG_SOUND4CNT_L = notes[3]->length |
        (notes[3]->envelope_step << 8) |
        SOUND4ENVDEC |
        (notes[3]->volume << 12);
    REG_SOUND4CNT_H = 3 |
        SOUND4STEPS15 |
        SOUND4PLAYONCE; */

    REG_SOUND1CNT_X |= 0x8000;
    // REG_SOUND2CNT_H |= 0x8000;
    // REG_SOUND3CNT_X |= 0X8000;
    // REG_SOUND4CNT_H |= 0X8000;

    // step++;
    // if (step > 128) step = 0;
}
