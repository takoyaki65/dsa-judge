#ifndef INCLUDE_GUARD_TEST_LINKED_LIST_H
#define INCLUDE_GUARD_TEST_LINKED_LIST_H
#include <string>

extern "C" {
#include "linked_list.h"
}

void test_insert_cell(Cell *p, int d);
void test_insert_cell_top(int d);
void test_delete_cell(Cell *p);
void test_delete_cell_top(void);
void test_display(std::string &output, const int max_size);

#endif // INCLUDE_GUARD_TEST_LINKED_LIST_H
