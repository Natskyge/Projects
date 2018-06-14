/* Include the following operations:
 * Rope:
 *  - char rope_index(Rope *t1, size_t i)
 *  - Rope* rope_concat(Rope *t1, Rope *t2)
 *  - rope_split(Rope *t1, size_t i)
 *  - rope_insert(Rope *t1, size_t i, Rope *s)
 *  - rope_delete(Rope *t1, size_t i, size_t j)
 *  - rope_report(Rope *t1, size_t i, size_t j)
 **/



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Data type in list */
#define TYPE int

/* Structs */
typedef struct _Node {
	int balance_factor;
	size_t weight;
	char *string;
	struct _Node *left, *right, *parent;
} Node;

typedef struct _Rope {
	Node *root;
} Rope;

/* Function declarations */
char rope_index(Rope *t1, size_t i);
Rope* rope_concat(Rope *t1, Rope *t2);
rope_split(Rope *t1, size_t i);
rope_insert(Rope *t1, size_t i, Rope *s);
rope_delete(Rope *t1, size_t i, size_t j);
rope_report(Rope *t1, size_t i, size_t j);


int
main(void)
{
	int a = 10;
	void *ptr = &a;
	printf("%d", *(int *)ptr);
	return 0;
}
