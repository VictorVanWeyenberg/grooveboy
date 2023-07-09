#include "interrupt_control.h"
#include "dma.h"
#include "timer.h"

#ifndef __INTERRUPT_H
#define __INTERRUPT_H

void interrupt_handler(void) {
  if (REG_IF & INT_DMA0) {
    REG_IF |= INT_DMA0;
    dma_interrupt(0);
  }
  if (REG_IF & INT_DMA1) {
    REG_IF |= INT_DMA1;
    dma_interrupt(1);
  }
  if (REG_IF & INT_VBLANK) {
    /* vblank_interrupt(); // Only slide and change according to domain. */
    REG_IF = INT_VBLANK;
    REG_IFBIOS |= INT_VBLANK;
  }
  if (REG_IF & INT_TIMER0) {
    timer_interrupt(0);
    REG_IF = INT_TIMER0;
    REG_IFBIOS |= INT_TIMER0;
  }
  if (REG_IF & INT_TIMER3) {
    timer_interrupt(3);
    REG_IF = INT_TIMER3;
    REG_IFBIOS |= INT_TIMER3;
  }
}

#endif
