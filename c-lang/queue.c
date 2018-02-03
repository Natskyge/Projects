#include <stdio.h>
#include <stdlib.h>

#define TYPE int

typedef struct Item {
	TYPE data;
	struct Item *prev;
} Item;

typedef struct Queue {
	Item *top;
	Item *bot;
	size_t size;
	size_t max;
} Queue;

Queue*
queueInit(size_t max)
{
	Queue *newQueue = (Queue*)malloc(sizeof(Queue));
	
	newQueue->top = NULL;
	newQueue->bot = NULL;
	newQueue->size = 0;
	newQueue->max = max;
	
	return newQueue;
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
push(TYPE data, Queue *queue)
{
	if (!queue || queue->size >= queue->max) {
		return 1;
	} else {
		Item *newItem = makeItem(data, NULL);
		
		if (queue->size == 0)
			queue->top = newItem;
		
		if (queue->bot)
			queue->bot->prev = newItem;
		
		queue->bot = newItem;
		++queue->size;
		
		return 0;
	}
}

int
pop(Queue *queue)
{
	if (!queue || 0 >= queue->size) {
		return 1;
	} else {
		Item *oldTop = queue->top;
		queue->top = queue->top->prev;
		--queue->size;
		free(oldTop);
		return 0;
	}
}

TYPE
peek(Queue* queue)
{
	if (!queue || queue->size == 0) {
		/* No data */
	} else {
		return queue->top->data;
	}
}

int
clearQueue(Queue *queue)
{
	if (!queue || queue->size <= 0)
		return 1;
	
	while (queue->size > 0) {
		pop(queue);
	}
	
	return 0;
}

int
printQueue(Queue* queue)
{
	printf("Size: %d\n", queue->size);
	printf("Max:  %d\n", queue->max);
	printf("TYPE: %d\n\n", peek(queue));
	return 1;
}

int
main(void)
{
	/* A test of the data Stucture */
	Queue *myQueue = queueInit(10);
	printf("pop return code: %d\n", pop(myQueue));
	printQueue(myQueue);

	push(23, myQueue);
	printQueue(myQueue);

	pop(myQueue);
	printQueue(myQueue);

	for (int i = 1; push(i, myQueue) != 1; i++) {}
	printf("push return code: %d\n", push(23, myQueue));
	printQueue(myQueue);

	clearQueue(myQueue);
	printQueue(myQueue);

	myQueue = NULL;

	printf("push return code: %d\n", push(23, myQueue));
	printf("pop return code: %d\n", pop(myQueue));

	free(myQueue);

	return 0;
}
