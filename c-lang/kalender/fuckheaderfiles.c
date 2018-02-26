#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* Structs {{{*/
typedef struct Task {
	char *name;
	size_t v, v_i;
	size_t d;
	size_t t, t_i;
	size_t e, e_i;
	size_t score;
} Task;

typedef struct Item {
	Task *data;
	struct Item *prev;
} Item;

typedef struct List {
	Item *top;
	size_t size;
} List;
/*}}}*/

/* Functions {{{*/
bool sortBy_v(Task task1, Task task2);
bool sortBy_t(Task task1, Task task2);
bool sortBy_e(Task task1, Task task2);

Task makeTask(char *name, size_t v, size_t d, size_t t, size_t e);
void addTask(Task task, List *taskList);
void getIndexs(List *taskList);
List* sortTasks(List *taskList);

List* listInit(void);
Item* makeItem(Task *data, Item *prev);
int cons(Task *data, List *list);
int cdr(List *list);
Task* car(List *list);
int freeList(List *list);
Task* dumpToArray(List *list);

/* NOTE: Order in the form < */
void merge(Task *A, int mid, int high, bool (*order)(Task, Task));
void mergeSort(Task *A, int len, bool (*order)(Task, Task));
/*}}}*/

/* Tasks {{{*/
bool
sortBy_v(Task task1, Task task2)
{
	return task1.v < task2.v;
}

bool
sortBy_t(Task task1, Task task2)
{
	return task1.t < task2.t;
}

bool
sortBy_e(Task task1, Task task2)
{
	return task1.e < task2.e;
}

Task
makeTask(char *name, size_t v, size_t d, size_t t, size_t e)
{
	Task newTask;
	newTask.name = name;
	newTask.v = v;
	newTask.d = d;
	newTask.t = t;
	newTask.e = e;
	newTask.v_i = 0;
	newTask.t_i = 0;
	newTask.e_i = 0;
	newTask.score = 0;

	return newTask;
}

void
addTask(Task task, List *taskList)
{
	cons(&task, taskList);
}


/*}}}*/

/* Mergesort {{{*/
void
merge(Task *A, int mid, int high, bool (*order)(Task, Task))
{
	/* Init temp array */
	Task *x = malloc(high * sizeof(Task));
	
	/* h pointer from middel in A, m from bottom in A, k places in x */
	int m,h,k;
	for (h = mid, m = 0, k = 0; k < high; ++k) {
		x[k] = m >= mid          ? A[h++]  /* m has reached mid    */
			 : h >= high         ? A[m++]  /* h has reached high   */
			 : order(A[h],A[m])  ? A[h++]  /* h < m, so we take h  */
			 : 				       A[m++]; /* h >= m, so we take m */
	}

	/* Dump x into A */
	for (int i = 0; i < high; i++) {
		A[i] = x[i];
	}

	free(x);
}

void
mergeSort(Task *A, int len, bool (*order)(Task, Task))
{
	/* Base case: 1 or 0 elements are always sorted */
	if (len < 2)
		return;

	int mid = len/2;

	/* sort left half and right half then merge */
	mergeSort(A,mid,order);
	mergeSort(A+mid,len-mid,order);
	merge(A,mid,len,order);
}
/*}}}*/

/* List {{{*/
List*
listInit(void)
{
	List *newList = (List*)malloc(sizeof(List));

	newList->top  = NULL;
	newList->size = 0;

	return newList;
}

Item*
makeItem(Task *data, Item *prev)
{
	Item *newItem = (Item*)malloc(sizeof(Item));

	newItem->data = data;
	newItem->prev = prev;

	return newItem;
}

int
cons(Task *data, List *list)
{
	Item *newItem = makeItem(data, list->top);
	list->top = newItem;
	++list->size;
	return 0;
}

int
cdr(List *list)
{
	Item *oldTop = list->top;
	list->top = list->top->prev;	
	--list->size;
	free(oldTop);
	return 0;
}

Task*
car(List *list)
{
	return list->top->data;
}

int
freeList(List *list)
{
	while (0 < list->size) {
		cdr(list);
	}

	free(list);
	return 0;
}

Task*
dumpToArray(List *list)
{
    int size = list->size;
    Task A[size];
    Item *itemPtr = list->top;

    for (int i = 0; i < size; ++i) {
        A[size-i-1] = *itemPtr->data;
        itemPtr = itemPtr->prev;
    }

    Task *r = A;
    return r;
}
/*}}} End List */

int
main(void)
{
	Task myTask = makeTask("lmao", 1, 2, 3, 4);
	Task myTask2 = makeTask("mao", 2, 3, 4, 5);
	List *myTaskList = listInit();
	addTask(myTask, myTaskList);
	printf("%d\n", myTaskList->top->data->v);
	if (sortBy_e(myTask, myTask2))
		printf("ayy lmao\n");

	return 1;
}
