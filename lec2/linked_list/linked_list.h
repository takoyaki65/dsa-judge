#ifndef INCLUDE_GUARD_LINKED_LIST_H
#define INCLUDE_GUARD_LINKED_LIST_H

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

#endif // INCLUDE_GUARD_LINKED_LIST_H
