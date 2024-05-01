BINARIES = link_test test_insert_cell_top test_insert_cell test_delete_cell_top test_delete_cell

CC=gcc
CPP=g++

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CPP) $(CFLAGS) -c -o $@ $<

link_test: main_linked_list.o linked_list.o
	$(CC) $(CFLAGS) -o $@ $^

test_insert_cell_top: test_insert_cell_top.o test_linked_list.o linked_list.o
	$(CPP) $(CFLAGS) -o $@ $^

test_insert_cell: test_insert_cell.o test_linked_list.o linked_list.o
	$(CPP) $(CFLAGS) -o $@ $^

test_delete_cell_top: test_delete_cell_top.o test_linked_list.o linked_list.o
	$(CPP) $(CFLAGS) -o $@ $^

test_delete_cell: test_delete_cell.o test_linked_list.o linked_list.o
	$(CPP) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(BINARIES)
