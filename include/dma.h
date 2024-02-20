#include <stdint.h>
#include <stdlib.h>

#ifndef __DMA_H
#define __DMA_H

typedef struct {
  void *src;
  void *dst;
  uint32_t cnt;
} DMA_CONTROLLER;

struct dma_entry {
  void *src;
  void *dst;
  uint32_t cnt;
};

#define DMA ((DMA_CONTROLLER *)0x040000B0)

#define DMA_DST_INC (0 << 21)
#define DMA_DST_DEC (1 << 21)
#define DMA_DST_FIX (2 << 21)

#define DMA_SRC_INC (0 << 23)
#define DMA_SRC_DEC (1 << 23)
#define DMA_SRC_FIX (2 << 23)
#define DMA_REPEAT  (1 << 25)
#define TRANSFER_16 (0 << 26)
#define TRANSFER_32 (1 << 26)
#define SPECIAL_START (3 << 28)
#define DMA_INT (1 << 30)
#define DMA_ON (1 << 31)

void dma_init();

void dma_interrupt(uint8_t dma_channel);

int dma_push(uint8_t dma_channel, void *src_address, uint16_t size,
             void *dst_address);

void dma_cycle();

#endif
