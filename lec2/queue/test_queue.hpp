#ifndef INCLUDE_GUARD_TEST_QUEUE_H
#define INCLUDE_GUARD_TEST_QUEUE_H
#include <string>

extern "C" {
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
}

void test_display(Queue *q, std::string &output, const int max_size);

#endif // INCLUDE_GUARD_TEST_QUEUE_H