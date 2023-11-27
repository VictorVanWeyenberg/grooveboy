#include "lnk.h"
#include <stdlib.h>
#include <string.h>

struct lnk_dq *create_lnk_dq(uint8_t m) {
	struct lnk_process **head = calloc(m, sizeof(struct lnk_process *));
	struct lnk_dq *s = calloc(1, sizeof(struct lnk_dq));
	s->head = head;
	s->max_size = m;
	s->size = 0;
	s->first = 0;
	return s;
}

uint8_t lnk_push(volatile struct lnk_dq *s, struct lnk_process *v) {
	if (s->size == s->max_size) return s->max_size;
	s->head[(s->first + s->size) % s->max_size] = v;
	return s->size++;
}

struct lnk_process *lnk_poll(volatile struct lnk_dq *s) {
	if (s->size == 0) return NULL;
	struct lnk_process *v = s->head[s->first];
	s->first++;
	s->first %= s->max_size;
	s->size--;
	return v;
}

volatile struct lnk_dq *lnk_queue;

void send_data(uint32_t data) {
    SIODATA32 = data;
    SIOCNT |= 0x0080;
}

void send_proc(struct lnk_process *proc) {
    uint32_t buffer;
    memcpy(&buffer, proc->data + proc->index, 4);
    send_data(buffer);
    proc->index += 4;
    if (proc->index == proc->length) {
        lnk_poll(lnk_queue);
        free(proc);
    }
}

void lnk_init() {
    SIOCNT = 0x5009;
    lnk_queue = create_lnk_dq(10);
}

void lnk_interrupt() {
    struct lnk_process *proc = lnk_queue->head[lnk_queue->first];
    send_proc(proc);
}

void send(void *data, uint32_t length) {
    send_data(0xAAAAAAAA);
    return;

    struct lnk_process *proc = calloc(1, sizeof(struct lnk_process));
    proc->data = data;
    proc->length = length;
    proc->index = 0;

    lnk_push(lnk_queue, proc);
    send_proc(proc);
    if (lnk_queue->size == 1) {
        // lnk_interrupt();
    }
}

uint32_t recv() {
   return 0;
}
