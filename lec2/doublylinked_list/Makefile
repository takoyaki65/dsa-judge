BINARIES = link_test

link_test: main_doublylinked_list.o doublylinked_list.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(BINARIES) *.txt
