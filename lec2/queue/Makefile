BINARIES = queue_test link_test test_push_pop_one test_push_pop_multiple test_push_pop_one_overlap test_push_pop_multiple_overlap

link_test: main_queue.o queue.o
	$(CC) $(CFLAGS) -o $@ $^

test_push_pop_one: test_push_pop_one.o test_queue.o queue.o
	g++ $(CFLAGS) -o $@ $^

test_push_pop_multiple: test_push_pop_multiple.o test_queue.o queue.o
	g++ $(CFLAGS) -o $@ $^

test_push_pop_one_overlap: test_push_pop_one_overlap.o test_queue.o queue.o
	g++ $(CFLAGS) -o $@ $^

test_push_pop_multiple_overlap: test_push_pop_multiple_overlap.o test_queue.o queue.o
	g++ $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(BINARIES) *.txt
