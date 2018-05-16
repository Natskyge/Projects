


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Data type in list */
#define TYPE int

/* Structs */
typedef struct Node {
	TYPE data;
	struct Node *prev;
	struct Node *next;
	bool sentinel;
} Node;

typedef struct List {
	Node *sentinel_node;
	size_t len;
} List;

/* Function declarations */
List *list_new(void);

Node *list_insert(List *list, size_t pos, Node *node);
Node *list_search(List *list, TYPE data);
Node *list_delete(List *list, size_t pos);

Node *list_push_back(List *list, Node *node);
Node *list_push_front(List *list, Node *node);

Node *list_pop_back(List *list);
Node *list_pop_front(List *list);

TYPE list_back(List *list);
TYPE list_front(List *list);

Node *list_head(List *list);
Node *list_tail(List *list);

Node *list_index(List *list, size_t pos);

size_t list_size(List *list);
bool list_empty(List *list);
bool list_end(Node *node);
List *list_concat(List *a, List *b);

void list_free(List *list);

Node *list_node_new(TYPE data);

static Node *list_node_link(Node *a, Node *b);
static Node *list_node_unlink(Node *b);
static Node *node_new(TYPE data, bool sentinel);
static void node_link(Node *node1, Node *node2);
static void error_msg(const char *err_msg);

/* Function implementations */
List*
list_new(void)
{
	List *new_list = (List*)malloc(sizeof(List));

	if (!new_list) {
		perror("list_new()");
		return NULL;
	}
	
	Node *sentinel_node = node_new(0, true);

	if (!sentinel_node) {
		error_msg("list_new(): Error creating sentinel_node");
		free(new_list);
		return NULL;
	}

	new_list->sentinel_node = sentinel_node;
	new_list->len = 0;
	node_link(new_list->sentinel_node,new_list->sentinel_node);
	
	return new_list;
}

Node*
list_insert(List *list, size_t pos, Node *node)
{
	if (!list) {
		error_msg("list_insert(): Invalid null pointer argument");
		return NULL;
	}

	node = list_node_link(node,list_index(list,pos));
	++list->len;

	return node;
}

Node*
list_search(List *list, TYPE data)
{
	Node* ptr = list_head(list);
	while (!list_end(ptr)) {
		if (ptr->data == data) 
			return ptr;

		ptr = ptr->next;
	}

	return NULL;
}

Node*
list_delete(List *list, size_t pos)
{
	if (!list) {
		error_msg("list_delete(): Invalid null pointer argument");
		return NULL;
	}

	Node *node = list_node_unlink(list_index(list, pos));
	--list->len;

	return node;
}

Node *list_push_back(List *list, Node *node)
{
	return list_insert(list,list_size(list),node);
}

Node *list_push_front(List *list, Node *node)
{
	return list_insert(list,0,node);
}

Node*
list_pop_back(List *list)
{
	return list_delete(list,list_size(list)-1);
}


Node*
list_pop_front(List *list)
{
	return list_delete(list,0);
}

TYPE
list_back(List *list)
{
	return list_tail(list)->data;
}

TYPE list_front(List *list)
{
	return list_head(list)->data;
}

Node*
list_head(List *list)
{
	if (!list) {
		error_msg("list_head(): Invalid null pointer argument");
		return NULL;
	}

	if (!list_end(list->sentinel_node)) {
		error_msg("list_head(): Malformed sentinel_node");
		return NULL;
	}

	return list->sentinel_node->next;
}

Node*
list_tail(List *list)
{
	if (!list) {
		error_msg("list_tail(): Invalid null pointer argument");
		return NULL;
	}

	if (!list_end(list->sentinel_node)) {
		error_msg("list_tail(): Malformed sentinel_node");
		return NULL;
	}

	return list->sentinel_node->prev;
}

Node*
list_index(List *list, size_t pos)
{
	int len = list_size(list);

	if (list->len < pos) {
		error_msg("get_node(): Position out of bounds.");
		return NULL;
	}

	Node *ptr = NULL;

	if (pos <= len/2) {
		ptr = list_head(list);

		for (int i = 0; i < pos; i++) {
			ptr = ptr->next;
		}
	} else {
		ptr = list->sentinel_node;
		for (int i = len; pos < i; i--) {
			ptr = ptr->prev;
		}
	}

	return ptr;
}

size_t
list_size(List *list)
{
	if (!list) {
		error_msg("list_size(): Invalid null pointer argument");
		return 0;
	}

	return list->len;
}

bool
list_empty(List *list)
{
	return (list_size(list) == 0);
}

bool
list_end(Node *node)
{
	if (!node) {
		error_msg("list_end(): Invalid null pointer argument");
		return false;
	}

	return node->sentinel;
}
List*
list_concat(List *a, List *b)
{
	if (a && !b)
		return a;
	if (b && !a)
		return b;

	if (!a && !b) {
		error_msg("list_concat(): no list given");
		return NULL;
	}

	node_link(list_tail(a),list_head(b));
	node_link(list_tail(b),a->sentinel_node);

	a->len += b->len;

	free(b->sentinel_node);
	free(b);

	return a;
}


void
list_free(List *list)
{
	while (!list_empty(list)) {
		Node *node = list_pop_back(list);
		free(node);
	}

	free(list->sentinel_node);
	free(list);
}


Node*
list_node_new(TYPE data)
{
	Node *new_node = node_new(data,false);
	return new_node;
}

static Node*
list_node_unlink(Node *node)
{
	if (node->sentinel)
		return NULL;

	Node *prev_node = node->prev;
	Node *next_node = node->next;

	node_link(prev_node,next_node);

	return node;
}

static Node*
list_node_link(Node *node, Node *next_node)
{
	if (!node) {
		error_msg("list_node_link(): Invalid NULL node");
		return NULL;
	}

	if (!next_node) {
		error_msg("list_node_link(): Invalid NULL next_node");
		return NULL;
	}

	node_link(next_node->prev,node);
	node_link(node,next_node);

	return node;
}

static Node*
node_new(TYPE data, bool sentinel)
{
	Node *new_node = (Node*)malloc(sizeof(Node));
	
	new_node->data = data;
	new_node->prev = NULL;
	new_node->next = NULL;
	new_node->sentinel = sentinel;
	
	return new_node;
}

static void
node_link(Node *node1, Node *node2)
{
	node1->next = node2;
	node2->prev = node1;
}

static void
error_msg(const char* err_msg)
{
	puts(err_msg);
	exit(EXIT_FAILURE);
}

void
print_list(List *list)
{
	Node *ptr = list->sentinel_node->next;
	
	while (!ptr->sentinel) {
		printf("%d, ", ptr->data);
		ptr = ptr->next;
	}
	
	printf("\n");
	
	return;
}

int
main(void)
{
	List *my_list = list_new(); // works

	for (int i = 0; i<11; i++) {
		list_insert(my_list,0,list_node_new(i)); // works
	}
	print_list(my_list); // works

	printf("Search: %d\n", list_search(my_list,5)->data); // Works

	list_delete(my_list,4); // works
	print_list(my_list);

	list_push_back(my_list,list_node_new(21)); // Works
	print_list(my_list);

	list_push_front(my_list,list_node_new(22)); // Works
	print_list(my_list);

	list_pop_front(my_list); // Works
	print_list(my_list);

	list_pop_back(my_list); // Works
	print_list(my_list);

	printf("Back: %d\n", list_back(my_list)); // works
	printf("Front: %d\n", list_front(my_list)); // works

	printf("Tail: %d\n", list_tail(my_list)->data); // works
	printf("Head: %d\n", list_head(my_list)->data); // works

	printf("At 3: %d\n", list_index(my_list,3)->data); // works

	printf("Size: %zu\n", list_size(my_list)); // works
	printf("Empty: %d\n", list_empty(my_list)); // works

	printf("Is 3 end: %d\n", list_end(list_index(my_list,3))); // works
	printf("Is sentinel end: %d\n", list_end(list_head(my_list)->prev)); // works

	List *nu_list = list_new();
	for (int i = 0; i<11; i++) {
		list_push_front(nu_list,list_node_new(-i));
	}
	print_list(nu_list);

	list_concat(my_list,nu_list); // Works

	print_list(my_list);

	list_free(my_list);

	printf("Succes!");
	return 0;
}
