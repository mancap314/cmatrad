CC = gcc
CARGS = -ljansson
PREREQUISITES = utils.h utils.c test.c


test.o: $(PREREQUISITES)
	$(CC) $^ -o $@ $(CARGS)

test-debug.o: $(PREREQUISITES)
	echo $<
	$(CC) -g -O0 $^ -o $@ $(CARGS)


.PHONY: clean test
test: 
	$(CC) $(PREREQUISITES) -o test.o $(CARGS)
	./test.o
	rm test.o
clean:
	rm -f *.o
