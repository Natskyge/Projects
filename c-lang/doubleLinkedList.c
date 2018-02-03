#include <stdio.h>
#include <stdlib.h>

#define TYPE int

typedef struct Node {
	TYPE data;
	struct Node *prev;
	struct Node *next;
} Node;

typedef struct List {
	size_t len;
	struct Node *first;
	struct Node *last;
} List;

Node*
makeNode(TYPE data, Node *next, Node *prev)
{
	Node *newNode = (Node*)malloc(sizeof(Node));

	newNode->data = data;
	newNode->prev = prev;
	newNode->next = next;

	return newNode;
}

List*
listInit(void)
{
	List *newList = (List*)malloc(sizeof(List));

	Node *first = makeNode(0,NULL,NULL);
	Node *last = makeNode(0,first,NULL);
	first->next = last;

	newList->len = 0;
	newList->first = first;
	newList->last = last;

	return newList;
}

int
main(void)
{
	List* testList = listInit();
	printf("First: %d\n", testList->first->data);
	printf("Last: %d\n", testList->last->data);

	/* Causes a segmentation fault for some bizarre reason. */
	printf("First from last: %d\n", testList->last->prev->data);
	printf("Last from first: %d\n", testList->first->next->data);
}
