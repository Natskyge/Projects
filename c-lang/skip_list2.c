#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <limits.h>

#define MAX_HEIGHT 16 /** Madx node height */

typedef struct Node {
	size_t height;           /** Amount of conections     */
	struct Node **conections; /** Array with forward nodes */
	int data;               /** Arbitary data            */
	int key;
} Node;

typedef struct Skip_list {
	size_t len;                     /** Amount of elements?                          */
	Node *header;                    /** Provides access to nodes                     */
} Skip_list;

static size_t
get_height(void) 
{
	int i;

    for(i = 1; i < MAX_HEIGHT && rand() % 2 == 0; i++); 

    return i;
}

bool
default_compare(void *a, void *b)
{
	return (*(int*)a < *(int*)b);
}

void
default_delete(void *a)
{
	free(a);
}

Node*
node_new(int data, int key, size_t height)
{
	Node *node = (Node*)malloc(sizeof(Node));
	Node **conections = malloc(height * sizeof(Node*));

	for (int i = 0; i<height; i++) {
		conections[i] = NULL;
	}

	node->height = height;
	node->conections = conections;
	node->data = data;
	node->key = key;

	return node;
}

Skip_list*
skip_list_new(void)
{
	/* Init skip list with sentinel nodes to remove special cases */
	Skip_list *skip_list = (Skip_list*)malloc(sizeof(Skip_list));
	
	skip_list->len = 0;
	skip_list->header = node_new(0,INT_MIN,MAX_HEIGHT);
	Node *tail = node_new(0,INT_MAX,MAX_HEIGHT);

	for (int i = 0; i<MAX_HEIGHT; i++) {
		skip_list->header->conections[i] = tail;
	}

	return skip_list;
}

Skip_list*
skip_list_insert(Skip_list *skip_list, int data, int key)
{
	size_t level = MAX_HEIGHT-1;
	Node *cur_node = skip_list->header;
	printf("%zu\n", get_height());
	Node *node = node_new(data,key,get_height());
	printf("%d\n",skip_list->header->conections[15]->key);

	while (true) {
		while (cur_node->conections[level]->key < key) { /* Error here */
			Node *next_node = cur_node->conections[level];
			if (node->height >= level) {
				cur_node->conections[level] = node;
			}
			cur_node = next_node;
		}

		if (level == 0) {
			node->conections[0] = cur_node;
			break;
		} else {
			--level;
		}
	}
	++skip_list->len;

	return skip_list;
}

Node*
skip_list_search(Skip_list *skip_list, int key)
{
	size_t level = MAX_HEIGHT-1;
	Node *cur_node = skip_list->header;

	while (true) {
		while (cur_node->conections[level]->key < key) { /* Error here */
			cur_node = cur_node->conections[level];
		}

		if (level == 0) {
			return cur_node;
			break;
		} else {
			--level;
		}
	}
	++skip_list->len;
}

int main(void) 
{
	Skip_list *skip_list = skip_list_new();
	printf("%d\n",skip_list->header->conections[15]->key);

	for (int i = 0; i<10; i++) {
		if (i != 5) {
			skip_list_insert(skip_list,i,i);
		} else {
			skip_list_insert(skip_list,i,i);
		}
	}

	Node *node = skip_list_search(skip_list,5);
	printf("%d\n", node->data);


	return 0;
}
