#include "test_doublylinked_list.hpp"
extern "C" {
#include "doublylinked_list_io.h"
}
#include <iostream>

int main() {
  Cell *head = test_CreateCell(0, true);

  ReadFromFile(head, "test_ReadFromFile.txt");

  std::string output;
  test_Display(head, output, 1024);

  if (output != "1 2 4 8 16 32 64 \n") {
    std::cerr << "Output is not correct." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Output is correct.\n";
  return 0;
}