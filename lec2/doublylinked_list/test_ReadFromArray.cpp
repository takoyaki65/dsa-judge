#include "test_doublylinked_list.hpp"

extern "C" {
#include "doublylinked_list_io.h"
}
#include <iostream>
#include <string>

int main() {
  int a[] = {1, 2, 3, 4, 5};
  int n = sizeof(a) / sizeof(a[0]);

  Cell *head = test_CreateCell(0, true);

  ReadFromArray(head, a, n);

  std::string output;
  test_Display(head, output, 1024);

  if (output != "1 2 3 4 5 \n") {
    std::cerr << "Output is not correct." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Output is correct.\n";
  return 0;
}
