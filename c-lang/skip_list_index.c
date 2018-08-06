#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <limits.h>

/* TODO: Remove depends on INT_MAX and INT_MIN and implement imutable
 * concat and split using the header as a wall and adjusting key when
 * traversing it using max_key and min_key. Make it indexable using
 * using skip_size.
 *
 * Or just index by how many nodes that are before a given node.
 * Or combine to calculate skip_size
 * Or save a bonus to length that gets added to nodes after the given node 
 * when traversing later.
 **/

#define MAX_HEIGHT 20 /** Max node height */

#define MIN(a, b)  ((a) > (b) ? (b) : (a))
#define MAX(a, b)  ((a) < (b) ? (b) : (a))

typedef struct Node {
	size_t height;           /** Amount of shortcuts      */
	struct Node **shortcuts; /** Array with forward nodes */
	int data;                /** Arbitary data            */
	int key;                 /** Search key               */
} Node;

typedef struct Skip_list {
	size_t len;   /** Amount of elements       */
	Node *header; /** Provides access to nodes */
} Skip_list;


static size_t
get_height(void) 
{
	int i;
	for(i = 1; i < MAX_HEIGHT &&  rand() % 2 == 0; i++){};
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
	Node **shortcuts = malloc(height * sizeof(Node*));

	node->height = height;
	node->shortcuts = shortcuts;
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
		skip_list->header->shortcuts[i] = tail;
	}

	return skip_list;
}

Skip_list*
skip_list_insert(Skip_list *skip_list, int data, int key)
{
	Node *cur_node = skip_list->header;
	Node *node = node_new(data,key,get_height());
	Node *update[MAX_HEIGHT];

	for (int level = MAX_HEIGHT-1; 0 <= level; --level) {
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

	for (int level = node->height-1; 0 <= level; --level) {
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

	for (int level = MAX_HEIGHT-1; 0 <= level; --level) {
		while (cur_node->shortcuts[level]->key < key) {
			cur_node = cur_node->shortcuts[level];
		}
		update[level] = cur_node;
	}

	if (cur_node->shortcuts[0]->key == key) {
		cur_node = cur_node->shortcuts[0];

		for (int level = cur_node->height-1; 0 <= level; --level) {
			Node *next_node = update[level]->shortcuts[level];
			if (next_node->key == key) {
				update[level]->shortcuts[level] = next_node->shortcuts[level];
			}
		}

		free(cur_node->shortcuts);
		free(cur_node);
		return skip_list;
	} else {
		return skip_list;
	}
}

Node*
skip_list_search(Skip_list *skip_list, int key)
{
	Node *cur_node = skip_list->header;

	for (int level = MAX_HEIGHT-1; 0 <= level; --level) {
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
skip_list_free(Node *node)
{
	Node *next;

	while (node) {
		next = node->shortcuts[0];
		free(node->shortcuts);
		free(node);
		node = next;
	}
}

int
main(void) 
{
	srand(time(NULL) + rand());
	Skip_list *skip_list = skip_list_new();
	int len = 5000000;
	int fails = 0;

	int keys[100] = {-998, -984, -982, -956, -940, -918, -887, -884, -873, -835,
		-813, -774, -755, -753, -750, -748, -700, -681, -677, -638, -595, -528,
		-503, -493, -488, -381, -377, -372, -362, -344, -309, -296, -287, -277,
		-256, -234, -209, -196, -159, -143, -112, -93, -91, -69, -64, -57, -39,
		-33, -25, -7, 6, 23, 29, 38, 40, 105, 106, 144, 218, 282, 286, 297, 307,
		331, 341, 357, 358, 367, 383, 386, 387, 399, 436, 450, 460, 469, 471,
		483, 493, 494, 495, 516, 521, 552, 617, 683, 692, 745, 770, 779, 803,
		806, 809, 835, 858, 860, 882, 935, 945, 983};

	int data[100] = {-935, -934, -925, -854, -850, -819, -817, -812, -787, -742,
		-734, -707, -649, -628, -620, -528, -492, -476, -470, -456, -432, -431,
		-413, -392, -390, -370, -364, -349, -346, -338, -331, -219, -202, -180,
		-169, -165, -156, -134, -128, -127, -83, -81, -57, -43, 4, 18, 48, 56,
		64, 70, 75, 94, 127, 181, 184, 193, 194, 201, 208, 256, 267, 277, 294,
		301, 316, 337, 341, 358, 363, 369, 385, 425, 440, 464, 522, 523, 527,
		532, 538, 562, 570, 620, 636, 649, 717, 739, 753, 762, 801, 816, 821,
		851, 883, 901, 922, 923, 947, 957, 966, 969};

	for (int i = 0; i < 100; i++) {
		skip_list_insert(skip_list,data[i],keys[i]);
	}

	for (int i = 0; i < 100; i++) {
		int search_data = skip_list_search(skip_list,keys[i])->data;

		if (search_data != data[i]) {
			++fails;
		}
	}

	printf("Fails: %d\n", fails);

	if (skip_list_search(skip_list,keys[50]) != NULL) {
		printf("Node exists\n");
	}

	skip_list_delete(skip_list,keys[50]);

	if (skip_list_search(skip_list,keys[50]) == NULL) {
		printf("Successful delete\n");
	}

	for (int i = 0; i < 100; i++) {
		if (i != 50) {
			int search_data = skip_list_search(skip_list,keys[i])->data;

			if (search_data != data[i]) {
				++fails;
			}
		}
	}

	printf("Fails: %d\n", fails);

	clock_t t;
	t = clock();

	for (int i = 1; i <= len; i++) {
		int dice = rand() % 4;
		if (dice == 0) {
			skip_list_delete(skip_list,(rand() % len) + 1);
		} else if (dice == 1) {
			skip_list_search(skip_list,(rand() % len) + 1);
		} else {
			skip_list_insert(skip_list,i,(rand() % len) + 1);
		}
	}

	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

	printf("Time %f\n", time_taken);

	skip_list_free(skip_list->header);

	//skip_list_free(skip_list);

	return 0;
}
