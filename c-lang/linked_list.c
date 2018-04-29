#include <stdio.h>
#include <stdlib.h>

#define TYPE int
#define NORMAL 1
#define FIRST 2
#define LAST 3
#define BEFORE 4
#define AFTER 5

typedef struct Node {
	TYPE data;
	struct Node *prev;
	struct Node *next;
	int node_type;
} Node;

typedef struct List {
	Node *first;
	Node *last;
	size_t len;
} List;

/* Function declrations */
Node* make_node(TYPE data, int node_type);
void link_nodes(Node *node1, Node *node2);
List* list_init(void);
void print_list(List *list);
Node* get_node(List *list, size_t pos);
void insert_after_node(TYPE data, List *list, size_t pos);
void insert_before_node(TYPE data, List *list, size_t pos);
void remove_node(List *list, size_t pos);

/* Function implementations */
Node*
make_node(TYPE data, int node_type)
{
	Node *new_node = (Node*)malloc(sizeof(Node));
	
	new_node->data = data;
	new_node->prev = NULL;
	new_node->next = NULL;
	new_node->node_type = node_type;
	
	return new_node;
}

void
link_nodes(Node *node1, Node *node2)
{
	node1->next = node2;
	node2->prev = node1;
}

List*
list_init(void)
{
	List *new_list  = (List*)malloc(sizeof(List));
	
	new_list->first = make_node(0, FIRST);
	new_list->last  = make_node(0, LAST);
	new_list->len   = 0;
	new_list->first->prev = NULL;
	new_list->last->next  = NULL;
	link_nodes(new_list->first,new_list->last);
	
	return new_list;
}

void
print_list(List *list)
{
	Node *ptr = list->first->next;
	
	while (ptr->node_type != LAST) {
		printf("%d, ", ptr->data);
		ptr = ptr->next;
	}
	
	printf("\n");
	
	return;
}

Node*
get_node(List *list, size_t pos)
{
	if (list->len < pos) {
		puts("Error: Postion out of bounds in linked list.");
		exit(EXIT_FAILURE);
	}
	
	Node *ptr = list->first->next;
	
	for (int i = 0; i < pos; i++) {
		ptr = ptr->next;
	}
	
	return ptr;
}

void
insert_node(TYPE data, List *list, size_t pos, int rel)
{
	Node *new_node = make_node(data,NORMAL);
	Node *before_node = rel == AFTER ? get_node(list,pos) : get_node(list,pos)->prev;
	Node *after_node  = before_node->next;

	link_nodes(before_node,new_node);
	link_nodes(new_node,after_node);
	++list->len;

	return;
}

void
remove_node(List *list, size_t pos)
{
	Node *node      = get_node(list, pos);
	Node *prev_node = node->prev;
	Node *next_node = node->next;
	
	link_nodes(prev_node,next_node);
	free(node);
	
	return;
}

int
main(void)
{
	List *my_list = list_init();
	insert_node(3,my_list,0,BEFORE);
	print_list(my_list);
	insert_node(2,my_list,0,BEFORE);
	print_list(my_list);
	insert_node(1, my_list, 0, BEFORE);
	print_list(my_list);
	insert_node(4,my_list,1,AFTER);
	print_list(my_list);
	remove_node(my_list, 3);
	print_list(my_list);

	printf("Succes!");
	return 0;
}
