#include "doublylinked_list.h"
#include <stdio.h>  // fprintf
#include <stdlib.h> // malloc

Cell *CreateCell(int d, bool is_head) {
  Cell *new_cell = (Cell *)malloc(sizeof(Cell));
  new_cell->data = d;
  new_cell->is_head = is_head;
  new_cell->prev = new_cell;
  new_cell->next = new_cell;
  return new_cell;
}

void InsertNext(Cell *this_cell, Cell *p) {
  p->next = this_cell->next;
  p->prev = this_cell;
  this_cell->next->prev = p;
  this_cell->next = p;
}

void InsertPrev(Cell *this_cell, Cell *p) {
  p->prev = this_cell->prev;
  p->next = this_cell;
  this_cell->prev->next = p;
  this_cell->prev = p;
}

void DeleteCell(Cell *this_cell) {
  if (!this_cell->is_head && this_cell->next == this_cell &&
      this_cell->prev == this_cell) {
    fprintf(stderr, "This cell is not connected to any other cells.\n");
  }
  this_cell->prev->next = this_cell->next;
  this_cell->next->prev = this_cell->prev;
  free(this_cell);
}

Cell *SearchCell(Cell *this_cell, int d) {
  Cell *p = this_cell;
  do {
    if (p->is_head == false && p->data == d) {
      return p;
    }
    p = p->next;
  } while (p != this_cell);
  return NULL;
}

void Display(Cell *this_cell) {
  Cell *p = this_cell;
  do {
    if (p->is_head == false) {
      fprintf(stdout, "%d ", p->data);
    }
    p = p->next;
  } while (p != this_cell);
  fprintf(stdout, "\n");
}
