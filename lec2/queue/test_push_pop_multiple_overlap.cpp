// テストする要件: キューに格納するデータが配列の末尾と先頭にまたがる場合で、
//              整数を複数連続して格納し、それが格納した順番で取り出せること
#include "test_queue.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <output_file>" << std::endl;
  }

  std::ofstream ofs(argv[1]);

  try {
    Queue *q = create_queue(4);
    enqueue(q, 1);
    enqueue(q, 2);
    enqueue(q, 3);
    dequeue(q);
    dequeue(q);
    dequeue(q);

    enqueue(q, 4);
    enqueue(q, 5);
    enqueue(q, 6);
    enqueue(q, 7);

    const std::vector<int> expected = {4, 5, 6, 7};
    std::vector<int> actual;
    for (int i = 0; i < 4; i++) {
      actual.push_back(dequeue(q));
    }

    if (actual != expected) {
      ofs << "FAIL: Expected [";
      for (int i = 0; i < 4; i++) {
        ofs << expected[i];
        if (i < 3) {
          ofs << ", ";
        }
      }
      ofs << "], but got [";
      for (int i = 0; i < 4; i++) {
        ofs << actual[i];
        if (i < 3) {
          ofs << ", ";
        }
      }
      ofs << "]" << std::endl;
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
