#include <stdio.h>
#include <stdlib.h>

#define TYPE int

typedef struct Item {
	TYPE data;
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
makeItem(TYPE data, Item *prev)
{
	Item *newItem = (Item*)malloc(sizeof(Item));
	
	newItem->data = data;
	newItem->prev = prev;
	
	return newItem;
}

int
push(TYPE data, Stack *stack)
{
	if (!stack || stack->size >= stack->max) {
		return 1;
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
	if (!stack || 0 >= stack->size) {
		return 1;
	} else {
		Item *oldTop = stack->top;
		stack->top = stack->top->prev;
		--stack->size;
		free(oldTop);
		return 0;
	}
}

TYPE
peek(Stack* stack)
{
	if (!stack || stack->size == 0) {
		/* No data */
	} else {
		return stack->top->data;
	}
}

int
clearStack(Stack *stack)
{
	if (!stack || stack->size <= 0)
		return 1;
	
	while (stack->size > 0) {
		pop(stack);
	}
	
	return 0;
}

int
printStack(Stack* stack)
{
	printf("Size: %d\n", stack->size);
	printf("Max:  %d\n", stack->max);
	printf("TYPE: %d\n\n", peek(stack));
	return 1;
}

int
main(void)
{
	/* A test of the data Stucture */
	Stack *myStack = stackInit(10);
	printf("pop return code: %d\n", pop(myStack));
	printStack(myStack);

	push(23, myStack);
	printStack(myStack);

	pop(myStack);
	printStack(myStack);

	for (int i = 1; push(i, myStack) != 1; i++) {}
	printf("push return code: %d\n", push(23, myStack));
	printStack(myStack);

	clearStack(myStack);
	printStack(myStack);

	myStack = NULL;

	printf("push return code: %d\n", push(23, myStack));
	printf("pop return code: %d\n", pop(myStack));

	free(myStack);

	return 0;
}
