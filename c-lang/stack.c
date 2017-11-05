#include <stdio.h>
#include <stdlib.h>

typedef struct Data {
	int data;
} Data;

typedef struct Item {
	Data data;
	struct Item *prev;
} Item;

typedef struct Stack {
	Item *top;
	size_t size;
	size_t max;
} Stack;

Stack*
stackInit(size_t max)
{
	Stack *newStack = (Stack*)malloc(sizeof(Stack));
	
	newStack->top = NULL;
	newStack->size = 0;
	newStack->max = max;
	
	return newStack;
}

Item*
makeItem(Data data, Item *prev)
{
	Item *newItem = (Item*)malloc(sizeof(Item));
	
	newItem->data = data;
	newItem->prev = prev;
	
	return newItem;
}

int
push(Data data, Stack *stack)
{
	if (stack->size >= stack->max) {
		return 1; /* Error, stackoverflow */
	} else {
		Item *newItem = makeItem(data, stack->top);
		stack->top = newItem;
		++stack->size;
		return 0;
	}
}

int
pop(Stack *stack)
{
	if (0 == stack->size) {
		return 1; /* Stack is empty */
	} else {
		Item *oldTop = stack->top;
		stack->top = stack->top->prev;
		--stack->size;
		free(oldTop);
		return 0;
	}
}

Data
peek(Stack* stack)
{
	return stack->top->data;
}

int
freeStack(Stack *stack)
{
	while (stack->size > 0) {
		pop(stack);
	}
	free(stack);
	return 0;
}

int
main(void)
{
	return 0;
}