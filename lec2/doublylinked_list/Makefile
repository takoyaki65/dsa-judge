BINARIES = link_test test_insert test_delete test_search

link_test: main_doublylinked_list.o doublylinked_list.o
	$(CC) $(CFLAGS) -o $@ $^

test_insert: test_insert.o test_doublylinked_list.o doublylinked_list.o
	g++ $(CFLAGS) -o $@ $^

test_delete: test_delete.o test_doublylinked_list.o doublylinked_list.o
	g++ $(CFLAGS) -o $@ $^

test_search: test_search.o test_doublylinked_list.o doublylinked_list.o
	g++ $(CFLAGS) -o $@ $^

test_ReadFromArray: test_ReadFromArray.o doublylinked_list.o test_doublylinked_list.o doublylinked_list_io.o
	g++ $(CFLAGS) -o $@ $^

test_WriteToArray: test_WriteToArray.o doublylinked_list.o test_doublylinked_list.o doublylinked_list_io.o
	g++ $(CFLAGS) -o $@ $^

test_ReadFromFile: test_ReadFromFile.o doublylinked_list.o test_doublylinked_list.o doublylinked_list_io.o
	g++ $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(BINARIES) *.txt
