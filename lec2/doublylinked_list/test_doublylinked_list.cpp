#include "test_doublylinked_list.hpp"
#include <cstdio>
#include <cstdlib>

Cell *test_CreateCell(int d, bool is_head) {
  Cell *cell = (Cell *)malloc(sizeof(Cell));
  cell->data = d;
  cell->is_head = is_head;
  cell->prev = cell;
  cell->next = cell;
  return cell;
}

void test_InsertNext(Cell *this_cell, Cell *p) {
  p->next = this_cell->next;
  p->prev = this_cell;
  this_cell->next->prev = p;
  this_cell->next = p;
}

void test_InsertPrev(Cell *this_cell, Cell *p) {
  p->prev = this_cell->prev;
  p->next = this_cell;
  this_cell->prev->next = p;
  this_cell->prev = p;
}

void test_DeleteCell(Cell *this_cell) {
  if (!this_cell->is_head && this_cell->next == this_cell &&
      this_cell->prev == this_cell) {
    fprintf(stderr, "This cell is not connected to any other cells.\n");
  }
  this_cell->prev->next = this_cell->next;
  this_cell->next->prev = this_cell->prev;
  free(this_cell);
}

Cell *test_SearchCell(Cell *this_cell, int d) {
  Cell *p = this_cell;
  do {
    if (p->is_head == false && p->data == d) {
      return p;
    }
    p = p->next;
  } while (p != this_cell);
  return NULL;
}

void test_Display(Cell *this_cell, std::string &output, const int max_size) {
  Cell *p = this_cell;
  do {
    if (p->is_head == false) {
      char buf[256];
      snprintf(buf, sizeof(buf), "%d ", p->data);
      output += buf;
      if (output.size() > max_size) {
        fprintf(stderr, "Output string exceeds the maximum size.\n");
        exit(EXIT_FAILURE);
      }
    }
    p = p->next;
  } while (p != this_cell);
  output += "\n";
}
