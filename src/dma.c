#include "dma.h"
#include "queue.h"
#include "dq.h"

uint8_t *cycle_counter;
struct dq **queues;
struct dma_entry **busy_entries;
const uint8_t cycles = 1;

void dma_init() {
  cycle_counter = calloc(4, sizeof(uint8_t));
  queues = calloc(4, sizeof(struct dq *));
  busy_entries = calloc(4, sizeof(struct dma_entry *));
  for (uint8_t i = 0; i < 4; i++) {
    cycle_counter[i] = cycles;
    queues[i] = create_dq(10);
  }
}

void dma_interrupt(uint8_t dma_channel) {
  if (dma_channel > 3) {
    return;
  }
  cycle_counter[dma_channel] = cycles;
  free(busy_entries[dma_channel]->src);
  free(busy_entries[dma_channel]);
}

int dma_push(uint8_t dma_channel, void *src_address, uint16_t size,
             void *dst_address) {
  if (dma_channel > 3) {
    return 0;
  }
  struct dma_entry *entry = calloc(1, sizeof(struct dma_entry));
  entry->src = src_address;
  entry->dst = dst_address;
  entry->cnt = size / 2; /* 16-bit system. */
  struct dq *queue = queues[dma_channel];
  return push(queue, entry);
}

void dma_cycle() {
  for (uint8_t channel = 0; channel < 4; channel++) {
    if (cycle_counter[channel]) {
      cycle_counter[channel]--;
      continue;
    }
    volatile struct dq *queue = queues[channel];
    if (queue->size == 0) {
      continue;
    }
    struct dma_entry *entry = poll(queue);
    DMA[channel].cnt = 0;
    DMA[channel].src = entry->src;
    DMA[channel].dst = entry->dst;
    if (channel == 1 || channel == 2) {
      DMA[channel].cnt = 0 | DMA_ON | TRANSFER_32 | SPECIAL_START | DMA_REPEAT;
    } else {
      DMA[channel].cnt = 0 | entry->cnt | DMA_SRC_INC | DMA_DST_INC | DMA_INT | TRANSFER_16 | DMA_ON;
    }
    busy_entries[channel] = entry;
  }
}
