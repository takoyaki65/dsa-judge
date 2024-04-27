#include "test_doublylinked_list.hpp"
#include <cstdio>

// SearchCell()関数によって、先頭、末尾、中間のセルを正しく検索できることを確認する
int main() {
  Cell *head = test_CreateCell(0, true);

  Cell* tail = test_CreateCell(9, false);
  test_InsertNext(head, tail);
  test_InsertNext(head, test_CreateCell(8, false));
  test_InsertNext(head, test_CreateCell(7, false));
  test_InsertNext(head, test_CreateCell(6, false));
  Cell* middle = test_CreateCell(5, false);
  test_InsertNext(head, middle);
  test_InsertNext(head, test_CreateCell(4, false));
  test_InsertNext(head, test_CreateCell(3, false));
  test_InsertNext(head, test_CreateCell(2, false));
  Cell *first = test_CreateCell(1, false);
  test_InsertNext(head, first);

  // list: 1 2 3 4 5 6 7 8 9

  // 先頭のセルを検索
  Cell *p = test_SearchCell(head, 1);
  if (p != first) {
    fprintf(stderr, "Search on beginning failed.\n");
    exit(EXIT_FAILURE);
  }

  // 末尾のセルを検索
  p = test_SearchCell(head, 9);
  if (p != tail) {
    fprintf(stderr, "Search on end failed.\n");
    exit(EXIT_FAILURE);
  }

  // 中間のセルを検索
  p = test_SearchCell(head, 5);
  if (p != middle) {
    fprintf(stderr, "Search on middle failed.\n");
    exit(EXIT_FAILURE);
  }

  printf("All tests passed.\n");
  return 0;
}
