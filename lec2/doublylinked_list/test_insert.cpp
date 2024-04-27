#include "test_doublylinked_list.hpp"
#include <string>

// セルを、リストの先頭、末尾、中間に挿入できることを確認する
int main() {
  Cell *head = test_CreateCell(0, true);

  // リストの先頭にセルを挿入
  InsertNext(head, test_CreateCell(3, false));
  InsertNext(head, test_CreateCell(2, false));
  InsertNext(head, test_CreateCell(1, false));

  std::string output;
  test_Display(head, output, 256);
  if (output != "1 2 3 \n") {
    printf("Insertion on beginning failed.\n");
    exit(EXIT_FAILURE);
  }

  // リストの末尾にセルを挿入
  InsertPrev(head, test_CreateCell(4, false));
  InsertPrev(head, test_CreateCell(5, false));
  InsertPrev(head, test_CreateCell(6, false));

  output.clear();
  test_Display(head, output, 256);
  if (output != "1 2 3 4 5 6 \n") {
    printf("Insertion on end failed.\n");
    exit(EXIT_FAILURE);
  }

  // リストの中間にセルを挿入
  Cell *p = test_SearchCell(head, 3);
  InsertNext(p, test_CreateCell(7, false));
  InsertNext(p, test_CreateCell(8, false));
  InsertNext(p, test_CreateCell(9, false));

  output.clear();
  test_Display(head, output, 256);
  if (output != "1 2 3 9 8 7 4 5 6 \n") {
    printf("Insertion on middle failed.\n");
    exit(EXIT_FAILURE);
  }

  printf("All tests passed.\n");
  return 0;
}
