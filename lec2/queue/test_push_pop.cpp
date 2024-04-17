// テストする要件:
// <size>個の要素を格納できるキューに対して、内部リングバッファの<pos>番目からス
// タートして、キューに<num>個の要素を連続して追加し、その後、キューから<num>個
// の要素を連続して取り出せること
#include "test_queue.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " <output_file> <size> <pos> <num>"
              << std::endl;
    return -1;
  }

  std::ofstream ofs(argv[1]);

  const int size = std::stoi(argv[2]);
  const int pos = std::stoi(argv[3]);
  const int num = std::stoi(argv[4]);

  try {
    Queue *q = create_queue(size);
    std::vector<int> expected(num);
    for (int i = 0; i < num; i++) {
      expected[i] = i;
    }

    // move to pos
    for (int i = 0; i < pos; ++i) {
      enqueue(q, i);
      dequeue(q);
    }

    for (int i = 0; i < num; i++) {
      enqueue(q, i);
    }

    std::vector<int> actual(num);
    for (int i = 0; i < num; i++) {
      actual[i] = dequeue(q);
    }

    if (expected != actual) {
      ofs << "FAIL: ";
      ofs << "Expected: ";
      for (int i = 0; i < num; i++) {
        ofs << expected[i] << " ";
      }
      ofs << "Actual: ";
      for (int i = 0; i < num; i++) {
        ofs << actual[i] << " ";
      }
      ofs << std::endl;
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
