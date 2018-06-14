#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>

typedef struct var {
	void *data;
	enum {
		list = 0,
		integer = 1 << 0,
		string = 1 << 1,
		boolean = 1 << 2
	} type;
} var;

typedef struct Item {
	var data;
	struct Item *prev;
} Item;

typedef struct List {
	Item *top;
	size_t size;
} List;

List*
nil(void)
{
	List *newList = (List*)malloc(sizeof(List));
	newList->top  = NULL;
	newList->size = 0;
	return newList;
}

Item*
new_item(var data, Item *prev)
{
	Item *newItem = (Item*)malloc(sizeof(Item));
	newItem->data = data;
	newItem->prev = prev;
	return newItem;
}

List*
cons(var data, List *list)
{
	List *new_list = list;
	Item *item = new_item(data, new_list->top);
	new_list->top = item;
	++new_list->size;
	return new_list;
}

List*
cdr(List *list)
{
	List *new_list = list;
	new_list->top = new_list->top->prev;
	--new_list->size;
	return new_list;
}

var
car(List *list)
{
	return list->top->data;
}

int
free_list(List *list)
{
	while (0 < list->size) {
		cdr(list);
	}
	
	free(list);
	return 0;
}

int
main(int argc, char** argv)
{
	return 0;
}
