#ifndef __QUEUE_H
#define __QUEUE_H

struct Queue {
  int size;
  int max_size;
  struct Node *head;
  struct Node *tail;
};

extern struct Queue *newQueue(int capacity);

extern int enqueue(struct Queue *q, void *value);

extern void *dequeue(struct Queue *q);

extern void freeQueue(struct Queue *q);

#endif
