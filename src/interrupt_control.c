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
  if (REG_IF & INT_TIMER3) {
    timer_interrupt(3);
    REG_IF |= INT_TIMER3;
  }
}

#endif
