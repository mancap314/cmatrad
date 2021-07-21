CC = gcc


test.o: utils.h utils.c test.c
	$(CC) $^ -o $@

test-debug.o: utils.h utils.c test.c
	echo $<
	$(CC) -g -O0 $^ -o $@


.PHONY: clean
clean:
	rm -f *.o
