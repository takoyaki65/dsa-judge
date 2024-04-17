#include <stdlib.h>

typedef struct cell {
  int data;
  struct cell *next;
} Cell;

extern Cell *head;

void insert_cell(Cell *p, int d);
void insert_cell_top(int d);
void delete_cell(Cell *p);
void delete_cell_top(void);
void display(void);

int main(void) {
  insert_cell_top(1);
  insert_cell(head, 3);
  insert_cell(head, 2);
  display();

  delete_cell(head);
  display();

  delete_cell_top();
  display();

  return EXIT_SUCCESS;
}
