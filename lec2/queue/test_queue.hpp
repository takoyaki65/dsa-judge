#ifndef INCLUDE_GUARD_TEST_QUEUE_H
#define INCLUDE_GUARD_TEST_QUEUE_H
#include <string>

extern "C" {
#include "queue.h"
}

void test_display(Queue *q, std::string &output, const int max_size);

#endif // INCLUDE_GUARD_TEST_QUEUE_H