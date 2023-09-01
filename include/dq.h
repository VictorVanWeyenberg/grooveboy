#ifndef DQ_H_
#define DQ_H_

#include "stdint.h"
#include "dma.h"

struct dq {
	uint8_t max_size;
	uint8_t size;
	uint8_t first;
	struct dma_entry **head;
};

struct dq *create_dq(uint8_t m);
uint8_t push(volatile struct dq *s, struct dma_entry *v);
struct dma_entry *pop(struct dq *s);
struct dma_entry *poll(volatile struct dq *s);
struct dma_entry *at(struct dq *s, uint8_t index);
void free_dq(struct dq *s);

#endif // DQ_H_
