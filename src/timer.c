#include "timer.h"
#include "math.h"
#include "io.h"
#include "cursor.h"
#include "snd.h"

const uint32_t CLK_FREQ = 16777216;
const double scalers[4] = { 1.0, 64.0, 256.0, 1024.0 };

void timer_start(uint8_t channel, int16_t start, uint8_t prescaler) {
    if (channel < 0 || channel > 3) {
        return;
    }
    TMxCNT[channel].enable = 0;
    TMxCNT[channel].start = -start;
    TMxCNT[channel].count_up = 0;
    TMxCNT[channel].prescaler = prescaler;
    TMxCNT[channel].irq_enable = 1;
    TMxCNT[channel].enable = 1;
}

void bpm_to_start(uint8_t channel, uint32_t bpm) {
    if (bpm < 16) {
        return;
    }
    for (uint8_t index = 0; index < 4; index++) {
        double scaler = scalers[index];
        double start = (CLK_FREQ / scaler) * (60.0 / bpm);
        if (start < 32768) {
            timer_start(channel, start, index);
            return;
        }
    }
}

void timer_interrupt(uint8_t channel) {
    if (channel == 3) {
        trigger_sound();
    }
}
