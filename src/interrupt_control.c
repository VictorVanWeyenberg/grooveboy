#include "interrupt_control.h"
#include "dma.h"
#include "timer.h"
#include "lnk.h"

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
    REG_IF |= INT_TIMER3;
    timer_interrupt(3);
  }
  if (REG_IF & INT_TIMER2) {
    REG_IF |= INT_TIMER2;
    timer_interrupt(2);
  }
  if (REG_IF & INT_TIMER1) {
    REG_IF |= INT_TIMER1;
    timer_interrupt(1);
  }
  if (REG_IF & INT_TIMER0) {
    REG_IF |= INT_TIMER0;
    timer_interrupt(0);
  }
  if (REG_IF & INT_COM) {
    REG_IF |= INT_COM;
    lnk_interrupt();
  }
}

#endif
