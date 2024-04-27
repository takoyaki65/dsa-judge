#ifndef INCLUDE_GUARD_TEST_DOUBLYLINKED_LIST_H
#define INCLUDE_GUARD_TEST_DOUBLYLINKED_LIST_H
#include <string>

extern "C" {
#include "doublylinked_list.h"
}

Cell *test_CreateCell(int d, bool is_head);
void test_InsertNext(Cell *this_cell, Cell *p);
void test_InsertPrev(Cell *this_cell, Cell *p);
void test_DeleteCell(Cell *this_cell);
Cell *test_SearchCell(Cell *this_cell, int d);
void test_Display(Cell *this_cell, std::string &output, const int max_size);

#endif // INCLUDE_GUARD_TEST_DOUBLYLINKED_LIST_H
