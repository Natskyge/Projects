/*Libaries*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Constants*/
#define LEFT 0
#define RIGHT 1

/*Structs*/
typedef struct node {
	char *data;
	struct node  *right;
	struct node  *left;
} node;

/*Variables*/
node *root;

/*Functions*/
node*
createNode(char *data, node *left, node *right)
{
	node *newNode = NULL;
	newNode = (node *)malloc(sizeof(node));
	
	if (newNode == NULL)
		return NULL;
	
	newNode->data  = data;
	newNode->left  = left;
	newNode->right = right;
	return newNode;
}

node*
findNode(int level, int row, node* root)
{
	int maxLeft = pow(2,level-1)-1;

	if (0 == level && 0 == row) {
		return root;
	} else if (root == NULL) {
		return root;
	} else if (maxLeft < row) {
		findNode(level-1, row-maxLeft-1, root->right);
	} else {
		findNode(level-1, row, root->left);
	}
}

int
insertNode(node *parent, char *data, int side)
{
	node *newNode = createNode(data, NULL, NULL);

	if (side == 0) {
		parent->left = newNode;
	} else if (side == 1) {
		parent->right = newNode;
	} else {
		return 1;
	}
}

/*main*/
int
main(void)
{
	root = createNode(NULL, NULL, NULL);
	return 1;
}
