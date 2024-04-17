#include "test_linked_list.hpp"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_insert_cell(Cell *p, int d) {
  // insert a cell after p
  assert(p != NULL);
  Cell *new_cell = (Cell *)malloc(sizeof(Cell));
  new_cell->data = d;
  new_cell->next = p->next;
  p->next = new_cell;
}

void test_insert_cell_top(int d) {
  Cell *new_cell = (Cell *)malloc(sizeof(Cell));
  new_cell->data = d;
  new_cell->next = head;
  head = new_cell;
}

void test_delete_cell(Cell *p) {
  // delete the cell after p
  assert(p != NULL && p->next != NULL);
  Cell *q = p->next;
  p->next = q->next;
  free(q);
}

void test_delete_cell_top(void) {
  // delete the top cell
  assert(head != NULL);
  Cell *q = head;
  head = q->next;
  free(q);
}

void test_display(std::string& output, const int max_size) {
  // display the linked list
  Cell *p = head;
  while(p != NULL) {
    output += std::to_string(p->data) + " ";
    p = p->next;
    if (output.size() > max_size) {
      output += "...\n";
      return;
    }
  }
  output += "\n";
}
