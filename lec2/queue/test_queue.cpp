#include "test_queue.hpp"

void test_display(Queue *q, std::string &output, const int max_size) {
  output = "";
  const int buffer_size = get_capacity(q);

  int front = q->front % buffer_size;
  int rear = q->rear % buffer_size;

  for (int idx = front; idx != rear; idx = (idx + 1) % buffer_size) {
    output += std::to_string(q->buffer[idx]) + " ";
    if (output.size() > max_size) {
      output += "...";
      break;
    }
  }
  
  output += "\n";
}
