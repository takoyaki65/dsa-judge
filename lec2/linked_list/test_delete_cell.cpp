#include "test_linked_list.hpp"
#include <exception>
#include <iostream>

int main() {
  try {
    test_insert_cell_top(1);
    test_insert_cell_top(2);
    test_insert_cell_top(2);
    test_insert_cell_top(3);
    test_insert_cell_top(4);

    // test delete_cell() from submitted code
    delete_cell(head->next->next);
    //
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  const std::string expected = "4 3 2 1 \n";
  std::string output;
  test_display(output, 100);

  if (output == expected) {
    std::cout << "PASS" << std::endl;
    return 0;
  } else {
    std::cout << "FAIL" << std::endl;
    return -1;
  }
  return 0;
}