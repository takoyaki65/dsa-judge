#include "doublylinked_list_io.h"
#include <stdio.h>

void ReadFromArray(Cell *head, int *d, int n) {
  int i;
  for (i = n - 1; i >= 0; i--) {
    InsertNext(head, CreateCell(d[i], false));
  }
}

int WriteToArray(Cell *head, int *d, int n) {
  int read_size = 0;
  Cell *p = head->next;
  while (read_size < n && p != head) {
    d[read_size] = p->data;
    read_size++;
    p = p->next;
  }
  return read_size;
}

void ReadFromFile(Cell *head, const char *fname) {
  FILE *fp = fopen(fname, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", fname);
    return;
  }

  int d;
  while (fscanf(fp, "%d", &d) != EOF) {
    InsertPrev(head, CreateCell(d, false));
  }

  fclose(fp);
}

void WriteToFile(Cell *head, const char *fname) {
  FILE *fp = fopen(fname, "w");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", fname);
    return;
  }

  Cell *p = head->next;

  while (p != head && p->next != head) {
    fprintf(fp, "%d ", p->data);
    p = p->next;
  }
  if (p != head) {
    fprintf(fp, "%d", p->data);
  }
  fprintf(fp, "\n");
  fclose(fp);
}
