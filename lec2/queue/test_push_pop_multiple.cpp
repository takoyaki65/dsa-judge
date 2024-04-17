// テストする要件: キューに整数を複数連続して格納し，それが格納した順番で取り出せること
#include "test_queue.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <output_file>" << std::endl;
    return -1;
  }

  std::ofstream ofs(argv[1]);

  try {
    const std::vector<int> expected = {1, 2, 3};
    std::vector<int> popped;
    Queue *q = create_queue(6);
    enqueue(q, 1);
    enqueue(q, 2);
    enqueue(q, 3);
    popped.push_back(dequeue(q));
    popped.push_back(dequeue(q));
    popped.push_back(dequeue(q));
    if (popped != expected) {
      ofs << "FAIL: Expected {1, 2, 3} but got {";
      for (size_t i = 0; i < popped.size(); i++) {
        ofs << popped[i];
        if (i != popped.size() - 1) {
          ofs << ", ";
        }
      }
      ofs << "}" << std::endl;
      delete_queue(q);
      ofs.close();
      return -1;
    }

    popped.clear();
    enqueue(q, 1);
    enqueue(q, 2);
    enqueue(q, 3);
    popped.push_back(dequeue(q));
    popped.push_back(dequeue(q));
    popped.push_back(dequeue(q));
    if (popped != expected) {
      ofs << "FAIL: Expected {1, 2, 3} but got {";
      for (size_t i = 0; i < popped.size(); i++) {
        ofs << popped[i];
        if (i != popped.size() - 1) {
          ofs << ", ";
        }
      }
      ofs << "}" << std::endl;
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
