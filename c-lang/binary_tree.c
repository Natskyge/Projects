/* Include the following operations:
 * Node:
 *  - node_insert(void *data)
 *  - node_search(void *data)
 *  - node_delete(size_t i)
 * Tree:
 *  - tree_concat(Tree *t1, Tree *t2)
 *  - tree_split(Tree *t1, size_t i)
 *
 **/

#define MAX(a, b)  ((a) < (b) ? (b) : (a))

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Data type in list */
#define TYPE int

/* Structs */
typedef struct _Node {
	int height, key;
	TYPE data;
	struct _Node *left, *right;
} Node;

/* Function declarations */
Node* node_rotate_left(Node *a);
Node* node_rotate_right(Node *a);
int node_height(Node *node);
void node_height_recalc(Node *node);
static void node_link_left(Node *parent, Node *left_child);
static void node_link_right(Node *parent, Node *right_child);
static Node* node_new(int key, TYPE data);
static void error_msg(const char *err_msg);

/* Function implementations */

Node*
node_insert(TYPE data, int key, Node *root)
{
	/* First case */
	return NULL;
}

Node*
node_balance(Node *node)
{
	if (!node) {
		return NULL;
	}

	node_height_recalc(node);

	if (node_height(node->right)-node_height(node->left) >= 2) {
		if (node_height(node->right->left) > node_height(node->right->right)) {
			node_rotate_right(node->right);
		}


		return node_rotate_left(node);
	} else if (node_height(node->right)-node_height(node->left) <= -2) {
		if (node_height(node->left->right) > node_height(node->left->left))
			node_rotate_left(node->left);
		
		return node_rotate_right(node);
	} else {
		return node;
	}
}

Node*
node_rotate_left(Node *a)
{
	Node *b = a->right;
	Node *t2 = b->left;
	node_link_right(a,t2);
	node_link_left(b,a);
	node_height_recalc(a);
	node_height_recalc(b);

	return b;
}

Node*
node_rotate_right(Node *a)
{
	Node *b = a->left;
	Node *t2 = b->right;
	node_link_left(a,t2);
	node_link_right(b,a);
	node_height_recalc(a);
	node_height_recalc(b);

	return b;
}

int
node_height(Node *node)
{
	return (!node) ? 0 : node->height;
}

void
node_height_recalc(Node *node)
{
	printf("D\n"); /* Error asigning node->height */
	printf("%d\n", node->key);
	node->height = 1+MAX(node_height(node->left),node_height(node->right));
}

static void
node_link_left(Node *parent, Node *left_child)
{
	parent->left = left_child;
}

static void
node_link_right(Node *parent, Node *right_child)
{
	parent->right = right_child;
}

Node*
node_new(int key, TYPE data)
{
	Node *node = (Node*)malloc(sizeof(Node));

	node->height = 1;
	node->key = key;
	node->data = data;
	node->left = NULL;
	node->right = NULL;

	return node;
}

Node*
avl_insert(Node* root, TYPE data, int key)
{
	Node *update[node_height(root)];
	Node *cur_node = root;
	int depth = 0;

	/* First case */
	if (!root) {
		root = node_new(key,data);
		return root;
	}

	while (cur_node) {
		if (cur_node->key == key) {
			cur_node->data = data;
			return root;
		} else if (key < cur_node->key) {
			if (!cur_node->left) {
				cur_node->left = node_new(key,data);
				break;
			}
			update[depth] = cur_node;
			++depth;
			cur_node = cur_node->left;
		} else {
			if (!cur_node->right) {
				cur_node->right = node_new(key,data);
				break;
			}
			update[depth] = cur_node;
			++depth;
			cur_node = cur_node->right;
		}
	}

	for (int i = depth; 0 <= i; --i) {
		node_balance(update[i]);
	}

	return root;
}

Node*
avl_search(Node *root, int key)
{
	Node *cur_node = root;
	while (cur_node) {
		if (key == cur_node->key) {
			return cur_node;
		} else if (cur_node->key < key) {
			cur_node = cur_node->right;
		} else {
			cur_node = cur_node->left;
		}
	}

	return NULL;
}

void
error_msg(const char* err_msg)
{
	puts(err_msg);
	exit(EXIT_FAILURE);
}

int
main(void)
{
	Node *root = node_new(1,1);
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

	avl_insert(root,2,2);

	//for (int i = 0; i < 100; i++) {
	//	avl_insert(root,data[i],keys[i]);
	//}

	//for (int i = 0; i < 100; i++) {
	//	int search_data = avl_search(root,keys[i])->data;

	//	if (search_data != data[i]) {
	//		++fails;
	//	}
	//}

	printf("Fails: %d\n", fails);
	return 0;
}
