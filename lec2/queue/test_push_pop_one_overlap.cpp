// テストする要件: キューに格納するデータが配列の末尾と先頭にまたがる場合で、
//              整数を一つ格納して取り出せること
#include "test_queue.hpp"
#include <exception>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <output_file>" << std::endl;
    return -1;
  }

  std::ofstream ofs(argv[1]);

  try {
    Queue *q = create_queue(3);
    enqueue(q, 1);
    int popped = dequeue(q);
    enqueue(q, 2);
    popped = dequeue(q);
    enqueue(q, 3);
    popped = dequeue(q);
    enqueue(q, 4);
    popped = dequeue(q);
    if (popped != 4) {
      ofs << "FAIL: Pushed 4 and popped " << popped << std::endl;
      delete_queue(q);
      ofs.close();
      return -1;
    }
    delete_queue(q);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  ofs << "PASS" << std::endl;
  ofs.close();
  return 0;
}
