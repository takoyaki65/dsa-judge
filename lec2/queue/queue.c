#include "queue.h"
#include <stdio.h>  // printf
#include <stdlib.h> // malloc, free

Queue *create_queue(int len) {
  Queue *q = (Queue *)malloc(sizeof(Queue));
  q->buffer = (int *)malloc(sizeof(int) * (len + 1));
  q->length = len + 1;
  q->front = 0;
  q->rear = 0;
  fprintf(stdout, "Queue created!\n");
  return q;
}

void enqueue(Queue *q, int d) {
  // If q is full, print an error message
  if ((q->rear + 1) % q->length == q->front) {
    fprintf(stderr, "overflow!\n");
    exit(EXIT_FAILURE);
  }
  q->buffer[q->rear] = d;
  q->rear = (q->rear + 1) % q->length;
}

int dequeue(Queue *q) {
  // If q is empty, print an error message
  if (q->front == q->rear) {
    fprintf(stderr, "underflow!\n");
    exit(EXIT_FAILURE);
  }
  int d = q->buffer[q->front];
  q->front = (q->front + 1) % q->length;
  return d;
}

void display(Queue *q) {
  int idx;
  for (idx = q->front; idx != q->rear && (idx + 1) % q->length != q->rear;
       idx = (idx + 1) % q->length) {
    printf("%d ", q->buffer[idx]);
  }
  if (idx != q->rear) {
    printf("%d", q->buffer[idx]);
  }
  printf("\n");
}

void delete_queue(Queue *q) {
  free(q->buffer);
  free(q);
  fprintf(stdout, "Queue deleted!\n");
}

int get_capacity(Queue *q) { return q->length; }
