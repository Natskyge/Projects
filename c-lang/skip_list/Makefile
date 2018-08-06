CC=gcc
CFLAGS=-I. -std=c99 -ledit
DEPS = skip_list.h
OBJ = test.o skip_list.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test_list: ${OBJ}
	$(CC) -o $@ $^ ${CFLAGS}
