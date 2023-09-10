#include "snd.h"
#include "tracker.h"
#include <math.h>

uint8_t step = 0;

void snd_init() {
    WAVE_RAM0_L = 0x1032;
    WAVE_RAM0_H = 0x5476;
    WAVE_RAM1_L = 0x98BA;
    WAVE_RAM1_H = 0xDCFE;
    WAVE_RAM2_L = 0x1032;
    WAVE_RAM2_H = 0x5476;
    WAVE_RAM3_L = 0x98BA;
    WAVE_RAM3_H = 0xDCFE;
}

uint32_t midi_key_to_rate(uint8_t key) {
    double octave = (key - 69.0) / 12.0;
    double freq = 440 * pow(2, octave);
    return round(2048 - 131072 / freq);
}

uint32_t midi_key_to_sample_rate(uint8_t key) {
    double octave = (key - 69.0) / 12.0;
    double freq = 440 * pow(2, octave);
    return round(2048 - 131072 / freq);
}

void trigger_sound() {
    step %= NOTES_PER_PATTERN;

    const struct note *note0 = get_note_of_instrument(0, step);
    uint16_t volume0 = note0->enabled ? ((note0->volume & 0xF) << 12) : 0;
    REG_SOUND1CNT_L = 0;
    REG_SOUND1CNT_H = (note0->length & 0x1F) | 0x80 | ((note0->envelope_step & 0x7) << 8) | volume0;
    REG_SOUND1CNT_X = 0x8000 | midi_key_to_rate(note0->index);

    const struct note *note1 = get_note_of_instrument(1, step);
    uint16_t volume1 = note1->enabled ? ((note1->volume & 0xF) << 12) : 0;
    REG_SOUND2CNT_L = note1->length | 0x80 | (note1->envelope_step << 8) | volume1;
    REG_SOUND2CNT_H = 0x8000 | midi_key_to_rate(note1->index);

    const struct note *note2 = get_note_of_instrument(2, step);
    REG_SOUND3CNT_L = 0x80;
    REG_SOUND3CNT_H = (1) | (note2->enabled << 15);
    REG_SOUND3CNT_X = 0x8000 | midi_key_to_sample_rate(note2->index);

    /* REG_SOUND4CNT_L = notes[3]->length |
        (notes[3]->envelope_step << 8) |
        SOUND4ENVDEC |
        (notes[3]->volume << 12);
    REG_SOUND4CNT_H = 3 |
        SOUND4STEPS15 |
        SOUND4PLAYONCE; */

    // REG_SOUND4CNT_H |= 0X8000;

    step++;
    if (step >= 128) step = 0;
}
