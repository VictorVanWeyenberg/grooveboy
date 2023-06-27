#include "dma.h"

uint8_t dmaing = 0;
struct Queue *queues[4];

void dma_init() {
  for (uint8_t i = 0; i < 4; i++) {
    queues[i] = newQueue(5);
  }
}

void dma_on(uint8_t dma_channel) {
  if (dmaing) {
    return;
  }
  dmaing = 1;
  dma_interrupt(dma_channel);
}

void dma_interrupt(uint8_t dma_channel) {
  if (dma_channel > 3) {
    return;
  }
  struct Queue *queue = queues[dma_channel];
  if (queue->size == 0) {
    dmaing = 0;
    return;
  }
  struct dma_entry *entry = dequeue(queue);
  DMA[dma_channel].src = entry->src;
  DMA[dma_channel].dst = entry->dst;
  DMA[dma_channel].cnt = 0 | entry->cnt | DMA_SRC_INC | DMA_DST_INC | DMA_INT | DMA_ON;
  free(entry);
}

int dma_push(uint8_t dma_channel, void *src_address, uint16_t size,
             void *dst_address) {
  if (dma_channel > 3) {
    return 0;
  }
  struct dma_entry *entry = malloc(sizeof(struct dma_entry));
  entry->src = src_address;
  entry->dst = dst_address;
  entry->cnt = size / 2; /* 16-bit system. */
  struct Queue *queue = queues[dma_channel];
  return enqueue(queue, entry);
}

