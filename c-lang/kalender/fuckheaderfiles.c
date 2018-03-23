#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

/* Structs {{{*/
typedef struct Task {
	char *name;
	size_t v, v_i;
	size_t d;
	size_t t, t_i;
	size_t e, e_i;
	float score;
} Task;

typedef struct Item {
	Task data;
	struct Item *prev;
} Item;

typedef struct List {
	Item *top;
	size_t size;
} List;
/*}}}*/

/* Functions {{{*/
/* Task */
/* NOTE: Order on tasks in the form < */
bool sortBy(Task task1, Task task2, int TYPE);
bool sortBy_v(Task task1, Task task2);
bool sortBy_t(Task task1, Task task2);
bool sortBy_e(Task task1, Task task2);
bool sortBy_score(Task task1, Task task2);
Task makeTask(char *name, size_t v, size_t d, size_t t, size_t e);
void addTask(Task task, List *taskList);
void sortTasks(List *taskList);

/* Input/Output */
void writeTaskList(List *taskList);
List* readTaskList(void);
void login(void);
void userInputLoop(void);
void shutdown(void);

/* List */
List* listInit(void);
Item* makeItem(Task data, Item *prev);
int cons(Task data, List *list);
int cdr(List *list);
Task car(List *list);
int freeList(List *list);
Task* dumpToArray(List *list);

/* Mergesortsort */
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

bool
sortBy_score(Task task1, Task task2)
{
	return task1.score < task2.score;
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
	cons(task, taskList);
}

void
sortTasks(List *taskList)
{
	/* Init */
	size_t len = taskList->size;
	Task *A = dumpToArray(taskList);

	/* Get function pointers */
	bool (*sortV)(Task, Task) = sortBy_v;
	bool (*sortT)(Task, Task) = sortBy_t;
	bool (*sortE)(Task, Task) = sortBy_e;
	bool (*sortScore)(Task, Task) = sortBy_score;

	/* Get V index */
	mergeSort(A, len, sortV);
	for (int i = 0; i<len; ++i) {
		A[i].v_i = i;
	}

	/* Get T index */
	mergeSort(A, len, sortT);
	for (int i = 0; i<len; ++i) {
		A[i].t_i = i;
	}
	
	/* Get E index */
	mergeSort(A, len, sortE);
	for (int i = 0; i<len; ++i) {
		A[i].e_i = i;
	}

	/* Calculate score and sort by score */
	for (int i = 0; i<len; ++i) {
		A[i].score = (4*A[i].v_i+2*A[i].t_i+A[i].e_i)/(abs(A[i].d-A[i].t)+0.1);
	}
	mergeSort(A, len, sortScore);
	
	/* Empty taskList */
	while (0 < taskList->size) {
		cdr(taskList);
	}

	/* Dump sorted array */
	for (int i = 0; i<len; ++i) {
		cons(A[i],taskList);
	}

	return;
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

/* Input/Output{{{*/

void
writeTaskList(List *taskList)
{
	/* Get array of tasks */
	Task *taskArray = dumpToArray(taskList);

	/* Open file */
	FILE *outfile;
	outfile = fopen("tasks.dat","wb+");

	if (outfile == NULL) {
		printf("Error reading file\n");
		exit(1);
	}

	/* Write array to file */
	fwrite(taskArray, sizeof(Task), taskList->size, outfile);
	fclose(outfile);
	return;
}


List*
readTaskList(void)
{
	FILE *infile;	
	//List *taskList;
	Task task;

	infile = fopen("tasks.dat","rb");

	if (infile == NULL) {
		printf("Error reading file\n");
		exit(1);
	}

	while(fread(&task, sizeof(Task), 1, infile)){
		printf("Score:%f\n", task.score);
	}
	fclose(infile);
	return NULL;
}

void
login(void)
{

}

void
userInputLoop(void)
{

}

void
shutdown(void)
{

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
makeItem(Task data, Item *prev)
{
	Item *newItem = (Item*)malloc(sizeof(Item));

	newItem->data = data;
	newItem->prev = prev;

	return newItem;
}

int
cons(Task data, List *list)
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

Task
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
        A[size-i-1] = itemPtr->data;
        itemPtr = itemPtr->prev;
    }

	Task *r = malloc(size*sizeof(Task));
	for (int i = 0; i < size; i++) {
		r[i] = A[i];
	}

    return r;
}
/*}}} End List */

int
main(void)
{
	List *myTaskList = listInit();

	Task myTask = makeTask("lmao", 4, 3, 2, 1);
	Task myTask2 = makeTask("mao", 2, 3, 4, 5);
	cons(myTask, myTaskList);
	cons(myTask2, myTaskList);

	for (int i=0;i<100000;++i) {
		cons(makeTask("aa", rand()%100000, rand()%100000, rand()%100000, rand()%100000), myTaskList);
	}

	sortTasks(myTaskList);

	writeTaskList(myTaskList);
	readTaskList();

	printf("Didn't crash!\n");
	printf("Score top: %f\n", myTaskList->top->data.score);
	printf("Score below: %f\n", myTaskList->top->prev->data.score);

	return 1;
}
