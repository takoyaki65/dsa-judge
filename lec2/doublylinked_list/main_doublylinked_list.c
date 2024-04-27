#include "doublylinked_list.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  Cell *head = CreateCell(0, true);
  printf("head: %p(d: %d, next: %p, prev: %p, is_head: %s)\n", head, head->data,
         head->next, head->prev, head->is_head ? "true" : "false");

  Cell *elem;

  InsertNext(head, CreateCell(2, false));
  InsertNext(head, CreateCell(1, false));
  InsertPrev(head, CreateCell(5, false));
  Display(head);

  elem = SearchCell(head, 2);
  InsertNext(elem, CreateCell(3, false));
  Display(head);

  elem = SearchCell(head, 5);
  DeleteCell(elem);
  Display(head);

  return EXIT_SUCCESS;
}
