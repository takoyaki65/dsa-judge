BINARIES = queue_test link_test test_push_pop

link_test: main_queue.o queue.o
	$(CC) $(CFLAGS) -o $@ $^

test_push_pop: test_push_pop.o test_queue.o queue.o
	g++ $(CFLAGS) -o $@ $^

test_underflow_sanitizer: test_underflow_sanitizer.o test_queue.o queue.o
	g++ $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(BINARIES) *.txt
