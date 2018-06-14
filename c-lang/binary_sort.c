#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ----- NOTES -----
 *
 * a
 *  \  <=> a<b
 *   b
 *
 *   a
 *  /  <=> b<=a
 * b
 *
 *     a
 *    /
 *   b   <=>   b
 *  /         / \
 * c         c   a
 *
 * a
 *  \
 *   b   <=>   b
 *    \       / \
 *     c     c   a
 *
 * Keep track of the last two nodes when inserting 
 * to know when to apply the above.
 *
 *  \  delete(a)  \
 *   a    =>       b
 *  /
 * b
 *
 *  \  delete(a)  \
 *   a    =>
 *
 *
 */

#define ROOT 0
#define NORMAL 1

typedef struct Node {
	int data;
	struct Node *left;
	struct Node *right;
	int type;
} Node;

Node*
make_node(int data, int type)
{
	Node *new_node = (Node*)malloc(sizeof(Node));
	
	new_node->data  = data;
	new_node->left  = NULL;
	new_node->right = NULL;
	new_node->type  = type;
	
	return new_node;
}

void
insert(int data, Node* root)
{
	if (data < root->data) {
		if (root->left == NULL) {
			root->left = make_node(data,NORMAL);
		} else {
			insert(data, root->left);
		}
	} else {
		if (root->right == NULL) {
			root->right = make_node(data,NORMAL);
		} else {
			insert(data, root->right);
		}
	}
}

Node*
build_tree(int *A, size_t len)
{
	Node* root = make_node(A[0],ROOT);

	for (int i = 1; i<len; ++i) {
		insert(A[i],root);
	}

	return root;
}

int*
make_array(Node* root, size_t len)
{
	int *A = (int*)malloc(len*sizeof(int));

	for (int i=0; i<len; ++i) {
		Node *ptr = root;
		Node **ptrp = &ptr;

		while (ptr->right != NULL) {
			ptr = ptr->right;
			ptrp = &ptr;
		}

		A[i] = ptr->data;
		*ptrp = ptr->left;
	}
	return A;
}

void
binary_sort(int *A, size_t len)
{
	int *X = make_array(build_tree(A,len),len);

	for (int i = 0; i<len; ++i) {
		A[i] = X[i];
	}
}

int
main(void)
{
	int A[7] = {3,5,1,2,0,9,8};
	binary_sort(A,7);
	for (int i =0; i<7; i++) {
		printf("%d\n", A[i]);
	}
	return 0;
}
