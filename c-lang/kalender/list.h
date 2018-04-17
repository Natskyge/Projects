#ifndef LIST_H_
#define LIST_H_

#include "list.h"
#include "kalender.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Item {
	Task *data;
	struct Item *prev;
} Item;

typedef struct List {
	Item *top;
	size_t size;
} List;


List*
listInit(void);
Item*
makeItem(Task *data, Item *prev);
int
cons(Task *data, List *list);
int
cdr(List *list);
Task*
car(List *list);
int
freeList(List *list);
Task*
dumpToArray(List *list);

#endif // FOO_H_
