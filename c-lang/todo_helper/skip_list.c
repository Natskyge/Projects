#include <stdint.h>
#include <limits.h>
#include "skip_list.h"
#include "task.h"

static uint8_t get_height(void);

Skip_list*
skip_list_new(void (*delete_)(void *))
{
	/* Init skip list with sentinel nodes to remove special cases */
	Skip_list *skip_list = (Skip_list*)malloc(sizeof(Skip_list));
	
	Task empty_task = {"",0,0,0,0,0};
	skip_list->len = 0;
	skip_list->header = node_new(empty_task,INT_MIN,MAX_HEIGHT);
	skip_list->delete_ = delete_;

	Node *tail = node_new(empty_task,INT_MAX,MAX_HEIGHT);

	for (uint8_t i = 0; i<MAX_HEIGHT; i++) {
		skip_list->header->shortcuts[i] = tail;
	}

	return skip_list;
}

Skip_list*
skip_list_insert(Skip_list *skip_list, Task data, int key)
{
	Node *cur_node = skip_list->header;
	Node *node = node_new(data,key,get_height());
	Node *update[MAX_HEIGHT];

	for (int8_t level = MAX_HEIGHT-1; 0 <= level; --level) {
		while (cur_node->shortcuts[level]->key < key) {
			cur_node = cur_node->shortcuts[level];
		}
		update[level] = cur_node;

		if (cur_node->shortcuts[level]->key == key) {
			cur_node->shortcuts[level]->data = data;
			++skip_list->len;
			return skip_list;
		}
	}

	for (int8_t level = node->height-1; 0 <= level; --level) {
		node->shortcuts[level] = update[level]->shortcuts[level];
		update[level]->shortcuts[level] = node;
	}

	++skip_list->len;

	return skip_list;
}

Skip_list*
skip_list_delete(Skip_list *skip_list, int key)
{
	Node *cur_node = skip_list->header;
	Node *update[MAX_HEIGHT];

	for (int8_t level = MAX_HEIGHT-1; 0 <= level; --level) {
		while (cur_node->shortcuts[level]->key < key) {
			cur_node = cur_node->shortcuts[level];
		}
		update[level] = cur_node;
	}

	if (cur_node->shortcuts[0]->key == key) {
		cur_node = cur_node->shortcuts[0];

		for (int8_t level = cur_node->height-1; 0 <= level; --level) {
			Node *next_node = update[level]->shortcuts[level];
			if (next_node->key == key) {
				update[level]->shortcuts[level] = next_node->shortcuts[level];
			}
		}
		--skip_list->len;
		return skip_list;
	} else {
		return skip_list;
	}
}

Node*
skip_list_search(Skip_list *skip_list, int key)
{
	Node *cur_node = skip_list->header;

	for (int8_t level = MAX_HEIGHT-1; 0 <= level; --level) {
		while (cur_node->shortcuts[level]->key <= key) {
			cur_node = cur_node->shortcuts[level];
		}
	}

	if (cur_node->key == key) {
		return cur_node;
	} else {
		return NULL;
	}
}

void
skip_list_free(Skip_list *skip_list)
{
	if (!skip_list) {
		/* Nothing to do */
		return;
	}
	Node *node = skip_list->header;
	Node *next;

	while (node) {
		next = node->shortcuts[0];
		free(node->shortcuts);
		free(node);
		node = next;
	}
}

Node*
node_new(Task data, int key, uint8_t height)
{
	Node *node = (Node*)malloc(sizeof(Node));
	Node **shortcuts = malloc(height * sizeof(Node*));

	node->height = height;
	node->shortcuts = shortcuts;
	node->data = data;
	node->key = key;

	return node;
}

static uint8_t
get_height(void) 
{
	uint8_t i;
	for(i = 1; i < MAX_HEIGHT &&  rand() % 2 == 0; i++){};
	return i;
}
