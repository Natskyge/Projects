
#include <stdio.h>
#include <stdlib.h>

#define TYPE int

typedef struct Item {
	TYPE data;
	struct Item *prev;
} Item;

typedef struct List {
	Item *top;
	size_t size;
} List;

List*
listInit(void)
{
	List *newList = (List*)malloc(sizeof(List));

	newList->top  = NULL;
	newList->size = 0;

	return newList;
}

Item*
makeItem(TYPE data, Item *prev)
{
	Item *newItem = (Item*)malloc(sizeof(Item));

	newItem->data = data;
	newItem->prev = prev;

	return newItem;
}

int
cons(TYPE data, List *list)
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

TYPE
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

int
main(void)
{
}
