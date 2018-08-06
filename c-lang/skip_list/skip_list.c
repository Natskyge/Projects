#include <stdint.h>
#include <limits.h>
#include "skip_list.h"

static uint8_t get_height(void);


Skip_list*
skip_list_new(void (*delete_)(void *))
{
	/* Init skip list with sentinel nodes to remove special cases */
	Skip_list *skip_list = (Skip_list*)malloc(sizeof(Skip_list));
	
	skip_list->len = 0;
	skip_list->header = node_new(0,INT_MIN,MAX_HEIGHT);
	skip_list->delete_ = delete_;

	Node *tail = node_new(0,INT_MAX,MAX_HEIGHT);

	for (uint8_t i = 0; i<MAX_HEIGHT; i++) {
		skip_list->header->shortcuts[i] = tail;
	}

	return skip_list;
}

Skip_list*
skip_list_insert(Skip_list *skip_list, void *data, int key)
{
	Node *cur_node = skip_list->header;
	Node *node = node_new(data,key,get_height());
	Node *update[MAX_HEIGHT];

	for (int8_t level = MAX_HEIGHT - 1; 0 <= level; --level) {
		while (cur_node->shortcuts[level]->key < key) {
			cur_node = cur_node->shortcuts[level];
		}

		/* Only necessary to update last node visited in layer. */
		update[level] = cur_node; 

		/* Edge case: Skip list already has node with key.
		 * Solution: Change data in the node to save space. */
		if (cur_node->shortcuts[level]->key == key) {
			cur_node->shortcuts[level]->data = data;
			++skip_list->len;
			return skip_list;
		}
	}

	for (int8_t level = node->height - 1; 0 <= level; --level) {
		node->shortcuts[level] = update[level]->shortcuts[level];
		update[level]->shortcuts[level] = node;
	}

	++skip_list->len;

	return skip_list;
}

/* TODO: Implement and test the below + add the necessary constructers */
//Skip_list*
//skip_list_insert(Skip_list *skip_list, void *data, int key)
//{
//	Node *cur_node = skip_list->header;
//	Node *node = node_new(data,key,get_height());
//	Node *update[MAX_HEIGHT];
//	int depth[MAX_HEIGHT];
//
//	for (int8_t i = 0; i < MAX_HEIGHT; ++i) {
//		depth[i] = 0;
//	}
//
//	for (int8_t level = MAX_HEIGHT - 1; 0 <= level; --level) {
//		while (cur_node->shortcuts[level]->key < key) {
//			cur_node = cur_node->shortcuts[level];
//			depth[level] += cur_node->skip_size[level];
//		}
//
//		/* Only necessary to update last node visited in layer. */
//		update[level] = cur_node; 
//
//		/* Edge case: Skip list already has node with key.
//		 * Solution: Change data in the node to save space. */
//		if (cur_node->shortcuts[level]->key == key) {
//			cur_node->shortcuts[level]->data = data;
//			++skip_list->len;
//			return skip_list;
//		}
//	}
//
//	for (int8_t level = 0; level < MAX_HEIGHT; ++level) {
//		if (level <= node->height - 1) {
//			node->shortcuts[level] = update[level]->shortcuts[level];
//			update[level]->shortcuts[level] = node;
//			node->skip_size[level] = cur_node->skip_size[level] - depth[level];
//			cur_node->skip_size[level] = depth[level];
//		} else {
//			++cur_node->skip_size[level];
//		}
//	}
//
//	++skip_list->len;
//
//	return skip_list;
//}

Skip_list*
skip_list_delete(Skip_list *skip_list, int key)
{
	Node *cur_node = skip_list->header;
	Node *update[MAX_HEIGHT];

	for (int8_t level = MAX_HEIGHT - 1; 0 <= level; --level) {
		while (cur_node->shortcuts[level]->key < key) {
			cur_node = cur_node->shortcuts[level];
		}
		update[level] = cur_node;
	}

	if (cur_node->shortcuts[0]->key == key) {
		cur_node = cur_node->shortcuts[0];

		for (int8_t level = cur_node->height - 1; 0 <= level; --level) {
			Node *next_node = update[level]->shortcuts[level];
			if (next_node->key == key) {
				update[level]->shortcuts[level] = next_node->shortcuts[level];
			}
		}
		return skip_list;
	} else {
		/* No node to delete. */
		return skip_list;
	}
}

Node*
skip_list_search(Skip_list *skip_list, int key)
{
	Node *cur_node = skip_list->header;

	for (int8_t level = MAX_HEIGHT - 1; 0 <= level; --level) {
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
	Node *node = skip_list->header;
	Node *next;

	while (node) {
		next = node->shortcuts[0];
		free(node->shortcuts);
		if (skip_list->delete_) {
			skip_list->delete_(node->data);
		}
		free(node);
		node = next;
	}
}

Node*
node_new(void *data, int key, uint8_t height)
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
	/* The chance of 1 is 1, the chance of n+1 is half n. */
	for(i = 1; i < MAX_HEIGHT &&  rand() % 2 == 0; i++){};
	return i;
}
