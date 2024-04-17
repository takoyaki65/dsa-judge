#include "linked_list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Cell *head = NULL;

void insert_cell(Cell *p, int d) {
  // insert a cell after p
  assert(p != NULL);
  Cell *new_cell = (Cell *)malloc(sizeof(Cell));
  new_cell->data = d;
  new_cell->next = p->next;
  p->next = new_cell;
}

void insert_cell_top(int d) {
  Cell *new_cell = (Cell *)malloc(sizeof(Cell));
  new_cell->data = d;
  new_cell->next = head;
  head = new_cell;
}

void delete_cell(Cell *p) {
  // delete the cell after p
  assert(p != NULL && p->next != NULL);
  Cell *q = p->next;
  p->next = q->next;
  free(q);
}

void delete_cell_top(void) {
  // delete the top cell
  assert(head != NULL);
  Cell *q = head;
  head = q->next;
  free(q);
}

void display(void) {
  Cell *p = head;
  while (p != NULL) {
    printf("%d", p->data);
    p = p->next;
    if (p != NULL) {
      printf(" ");
    }
  }
  printf("\n");
}
