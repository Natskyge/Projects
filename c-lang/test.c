#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
	char *data;
	struct node *left;
	struct node *right;
} Node;

Node *sof;
Node *eof;

Node*
createNode(char *data, Node *left, Node *right)
{
	Node *newNode = NULL;
	newNode = (Node *)malloc(sizeof(Node));
	
	if (newNode == NULL)
		return NULL;
	
	newNode->data  = data;
	newNode->left  = left;
	newNode->right = right;
	return newNode;
}

int
insertNode(char *data, Node *leftNode)
{
	Node *newNode = NULL;
	Node *rightNode = leftNode->right;
    newNode = createNode(data, leftNode, rightNode);
    
    if (newNode == NULL)
        return 1;

    leftNode->right	= newNode;
    rightNode->left = newNode;
    return 0;
}

char *test = "ayy lmao";

char* 
textSpan(char* text, int start, int stop)
{
	int textLen = strlen(text);
	if ( textLen < stop ) {
		return NULL;
	} else if ( start < 0 )
		return NULL;
	
	char *span;
	for (int index = 0; index + start < stop; index++)
		*(span+index) = *(text+start+index);

	return span;
}

Node*
listInit(void)
{
	sof = createNode(NULL, NULL, eof);
	eof = createNode(NULL, sof, NULL);
	sof->right = eof;
}

int
main(void)
{
	return 1;
}
