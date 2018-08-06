CC=gcc
CFLAGS=-I. -std=c99 -ledit
DEPS = skip_list.h task.h
OBJ = todo.o skip_list.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

todo: ${OBJ}
	$(CC) -o $@ $^ ${CFLAGS}
