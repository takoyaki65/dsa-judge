#ifndef INCLUDE_GUARD_QUEUE_H
#define INCLUDE_GUARD_QUEUE_H

typedef struct queue {
  int *buffer;
  int length;
  int front;
  int rear;
} Queue;

Queue *create_queue(int len);
void enqueue(Queue *q, int d);
int dequeue(Queue *q);
void display(Queue *q);
void delete_queue(Queue *q);
int get_capacity(Queue *q);

#endif // INCLUDE_GUARD_QUEUE_H
