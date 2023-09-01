#include "dq.h"
#include "stdio.h"
#include "stdlib.h"

struct dq *create_dq(uint8_t m) {
	struct dma_entry **head = calloc(m, sizeof(struct dma_entry *));
	struct dq *s = calloc(1, sizeof(struct dq));
	s->head = head;
	s->max_size = m;
	s->size = 0;
	s->first = 0;
	return s;
}

uint8_t push(volatile struct dq *s, struct dma_entry *v) {
	if (s->size == s->max_size) return s->max_size;
	s->head[(s->first + s->size) % s->max_size] = v;
	return s->size++;
}

struct dma_entry *pop(struct dq *s) {
	if (s->size == 0) return NULL;
	s->size--;
	return s->head[(s->first + s->size) % s->max_size];
}

struct dma_entry *poll(volatile struct dq *s) {
	if (s->size == 0) return NULL;
	struct dma_entry *v = s->head[s->first];
	s->first++;
	s->first %= s->max_size;
	s->size--;
	return v;
}

struct dma_entry *at(struct dq *s, uint8_t index) {
	if (s->size == 0) return NULL;
	return s->head[(s->first + index) % s->max_size];
}

void free_dq(struct dq *s) {
	free(s->head);
	free(s);
}
