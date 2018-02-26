#include "list.h"
#include "kalender.h"
#include <stdio.h>
#include <stdlib.h>

List*
listInit(void)
{
	List *newList = (List*)malloc(sizeof(List));

	newList->top  = NULL;
	newList->size = 0;

	return newList;
}

Item*
makeItem(Task *data, Item *prev)
{
	Item *newItem = (Item*)malloc(sizeof(Item));

	newItem->data = data;
	newItem->prev = prev;

	return newItem;
}

int
cons(Task *data, List *list)
{
	Item *newItem = makeItem(data, list->top);
	list->top = newItem;
	++list->size;
	return 0;
}

int
cdr(List *list)
{
	Item *oldTop = list->top;
	list->top = list->top->prev;	
	--list->size;
	free(oldTop);
	return 0;
}

Task*
car(List *list)
{
	return list->top->data;
}

int
freeList(List *list)
{
	while (0 < list->size) {
		cdr(list);
	}

	free(list);
	return 0;
}

Task*
dumpToArray(List *list)
{
    int size = list->size;
    Task A[size];
    Item *itemPtr = list->top;

    for (int i = 0; i < size; ++i) {
        A[size-i-1] = itemPtr->data;
        itemPtr = itemPtr->prev;
    }

    Task *r = A;
    return r;
}
