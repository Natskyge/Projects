#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <limits.h>

#define MAX_HEIGHT 20 /** Madx node height */

#define MIN(a, b)  ((a) > (b) ? (b) : (a))
#define MAX(a, b)  ((a) < (b) ? (b) : (a))

typedef struct Node {
	struct Node **forward; /** Array with forward nodes */
	struct Node **backward; /** Array with backward nodes */
	uint8_t height;           /** Amount of forward     */
	int data;               /** Arbitary data            */
	int key;
} Node;

typedef struct Skip_list {
	Node *header;                    /** Provides access to nodes                     */
	size_t len;                     /** Amount of elements?                          */
	int max_key;
	int min_key;
} Skip_list;

static uint8_t
get_height(void) 
{
	uint8_t i;

    for(i = 1; i < MAX_HEIGHT &&  rand() % 2 == 0; i++); 

    return i;
}


int
floor_avg(int x, int y)
{
	return (x + y) >> 1;
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
node_new(int data, int key, uint8_t height)
{
	Node *node = (Node*)malloc(sizeof(Node));
	Node **forward = malloc(height * sizeof(Node*));
	Node **backward = malloc(height * sizeof(Node*));

	//for (int i = 0; i<height; i++) {
	//	forward[i] = NULL;
	//}

	node->height = height;
	node->forward = forward;
	node->backward = backward;
	node->data = data;
	node->key = key;

	return node;
}

void
node_link(Node *prev_node, Node *next_node, uint8_t level)
{
	prev_node->forward[level] = next_node;
	next_node->backward[level] = prev_node;
}

Skip_list*
skip_list_new(void)
{
	/* Init skip list with sentinel nodes to remove special cases */
	Skip_list *skip_list = (Skip_list*)malloc(sizeof(Skip_list));
	
	skip_list->len = 0;
	skip_list->min_key = INT_MAX;
	skip_list->max_key = INT_MIN;
	skip_list->header = node_new(0,INT_MIN,MAX_HEIGHT);
	Node *tail = node_new(0,INT_MAX,MAX_HEIGHT);

	for (int i = 0; i<MAX_HEIGHT; i++) {
		node_link(skip_list->header,tail,i);
		node_link(tail,skip_list->header,i);
	}

	return skip_list;
}

/** TODO: FIX */
Skip_list*
skip_list_insert(Skip_list *skip_list, int data, int key)
{
	Node *node = node_new(data,key,get_height());

	if (key < floor_avg(skip_list->min_key,skip_list->max_key)) {
		Node *cur_node = skip_list->header;
		for (int level = MAX_HEIGHT-1; 0 <= level; --level) {
			while (cur_node->forward[level]->key < key) {
				cur_node = cur_node->forward[level];
			}
			if (level <= node->height - 1) {
				node_link(node,cur_node->forward[level],level);
				node_link(cur_node,node,level);
			}
		}
	} else {
		Node *cur_node = skip_list->header->backward[0];
		for (int level = MAX_HEIGHT-1; 0 <= level; --level) {
			while (key < cur_node->backward[level]->key) {
				cur_node = cur_node->backward[level];
			}
			if (level <= node->height - 1) {
				node_link(cur_node->backward[level],node,level);
				node_link(node,cur_node,level);
			}
		}
	}


	++skip_list->len;
	skip_list->min_key = MIN(skip_list->min_key,key);
	skip_list->max_key = MAX(skip_list->max_key,key);

	return skip_list;
}

void
node_free(Node *node)
{
	if (node) {
		free(node->forward);
		free(node);
	}
}

/** TODO: FIX */
Node*
skip_list_search(Skip_list *skip_list, int key)
{
	Node *cur_node;
	if (key < floor_avg(skip_list->min_key,skip_list->max_key)) {
		cur_node = skip_list->header;
		for (int level = MAX_HEIGHT-1; 0 <= level; --level) {
			while (cur_node->forward[level]->key < key) {
				cur_node = cur_node->forward[level];
			}
		}
	} else {
		cur_node = skip_list->header->backward[0];
		for (int level = MAX_HEIGHT-1; 0 <= level; --level) {
			while (key < cur_node->backward[level]->key) {
				cur_node = cur_node->backward[level];
			}
		}
	}

	return cur_node;
}

int
main(void) 
{
	srand(time(NULL) + rand());
	Skip_list *skip_list = skip_list_new();
	int len = 1000000;

	clock_t t;
	t = clock();

	for (int i = 1; i <= len; i++) {
		if (i == 5000) {
			skip_list_insert(skip_list,i,i);
		} else {
		skip_list_insert(skip_list,i,(rand() % len) + 1);
		}
		//printf("%d inserted!\n", i);
	}

	for (int i = 1; i <= len; i++) {
		skip_list_search(skip_list,(rand() % len) + 1);
		//printf("%d searched!\n", i);
	}

	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
 
    printf("fun() took %f seconds to execute \n", time_taken);

	printf("%d\n", skip_list_search(skip_list,5000)->data);

	return 0;
}
