// テストする要件:
// <size>個の要素を格納できるキューに対して、内部リングバッファの<pos>番目からス
// タートして、キューに<num>個の要素を連続して追加し、その後、キューから<num>+1
// 個の要素を連続し取り出そうとする。このとき、キューが空の状態で取り出そうとし
// た場合、アンダーフローを検知できるかどうかをテストする。
#include "test_queue.hpp"
#include <exception>
#include <fstream>
#include <iostream>

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

    // move to pos
    for (int i = 0; i < pos; ++i) {
      enqueue(q, i);
      dequeue(q);
    }

    for (int i = 0; i < num; i++) {
      enqueue(q, i);
    }

    for (int i = 0; i < num + 1; i++) {
      dequeue(q);
    }

    delete_queue(q);
  } catch (std::exception &e) {
    ofs << "Exception: " << e.what() << std::endl;
    ofs.close();
    return -1;
  }

  ofs << "FAIL: No exception" << std::endl;
  ofs.close();
  return -1;
}