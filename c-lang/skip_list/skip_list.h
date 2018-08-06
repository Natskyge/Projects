/*
 * skip_list.h - Interface for a skip list.
 *
 */

#ifndef LIST_H
#define LIST_H

#include <stdint.h>
#include <stdlib.h>

#ifndef MAX_HEIGHT
#define MAX_HEIGHT 16
#endif

typedef struct Node {
	uint8_t height;          /** Amount of shortcuts      */
	struct Node **shortcuts; /** Array with forward nodes */
	void *data;              /** Arbitary data            */
	int key;                 /** Search key               */
} Node;

typedef struct Skip_list {
	size_t len;              /** Amount of elements               */
	Node *header;            /** Provides access to nodes         */
	void (*delete_)(void *); /** Function for deleting void *data */
} Skip_list;

Skip_list* skip_list_new(void (*delete_)(void *));

Skip_list* skip_list_insert(Skip_list *skip_list, void *data, int key);
Skip_list* skip_list_delete(Skip_list *skip_list, int key);
Node* skip_list_search(Skip_list *skip_list, int key);

void skip_list_free(Skip_list *skip_list);

Node* node_new(void *data, int key, uint8_t height);

#endif /* LIST_H */
