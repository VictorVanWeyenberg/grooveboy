#ifndef __QUEUE_H
#define __QUEUE_H

struct Queue {
  int size;
  int max_size;
  struct Node *head;
  struct Node *tail;
};

struct Queue *newQueue(int capacity);

int enqueue(struct Queue *q, void *value);

void *dequeue(struct Queue *q);

void freeQueue(struct Queue *q);

#endif
