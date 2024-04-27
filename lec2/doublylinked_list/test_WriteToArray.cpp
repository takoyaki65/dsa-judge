#include "test_doublylinked_list.hpp"
extern "C" {
#include "doublylinked_list_io.h"
}
#include <iostream>

int main() {
  Cell *head = test_CreateCell(0, true);
  test_InsertNext(head, test_CreateCell(5, false));
  test_InsertNext(head, test_CreateCell(4, false));
  test_InsertNext(head, test_CreateCell(3, false));
  test_InsertNext(head, test_CreateCell(2, false));
  test_InsertNext(head, test_CreateCell(1, false));

  int a[6];
  int n = WriteToArray(head, a, (sizeof(a) / sizeof(a[0])));
  if (n != 5) {
    std::cerr << "Output is not correct." << std::endl;
    exit(EXIT_FAILURE);
  }

  if (a[0] != 1 || a[1] != 2 || a[2] != 3 || a[3] != 4 || a[4] != 5) {
    std::cerr << "Output is not correct." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Output is correct.\n";
  return 0;
}
