#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

struct timer_cnt {
    int16_t start;
    uint16_t prescaler: 2,
        count_up: 1,
        not_used_1: 3,
        irq_enable: 1,
        enable: 1,
        not_used_2: 8;
} __attribute__((packed, aligned(4)));

#define TMxCNT ((struct timer_cnt *) 0x4000100)

void timer_start(uint8_t channel, int16_t start, uint8_t prescaler);
void bpm_to_start(uint8_t channel, uint16_t bpm);
void timer_interrupt(uint8_t channel);

#endif // TIMER_H_
