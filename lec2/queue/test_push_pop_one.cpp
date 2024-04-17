// テストする要件: キューに整数を1つ格納し，それが取り出せること
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
    Queue *q = create_queue(2);
    enqueue(q, 1);
    int popped = dequeue(q);
    if (popped != 1) {
      ofs << "FAIL: Pushed 1 and popped " << popped << std::endl;
      ofs.close();
      return -1;
    }
    enqueue(q, 2);
    popped = dequeue(q);
    if (popped != 2) {
      ofs << "FAIL: Pushed 2 and popped " << popped << std::endl;
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
