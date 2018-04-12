#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

typedef struct Task {
	char name[140]; /* Name of task */
	size_t v, v_i;  /* Importance of task */
	time_t d;       /* Deadline of task */
	size_t t, t_i;  /* Time to complete task */
	size_t e, e_i;  /* Energy required by task */
	float score;    /* Score of task */
} Task;

typedef struct Item {
	Task data;         /* Data contained in item */
	struct Item *prev; /* Previous item */
} Item;

typedef struct List {
	Item *top;   /* Topmost item */
	size_t size; /* Size of list */
} List;

/* Task */
bool sortBy(Task task1, Task task2, int TYPE);
bool sortBy_v(Task task1, Task task2);
bool sortBy_t(Task task1, Task task2);
bool sortBy_e(Task task1, Task task2);
bool sortBy_score(Task task1, Task task2);
float scoreFunc(Task task);
Task makeTask(char *name, size_t v, time_t d, size_t t, size_t e);
void sortTasks(List *taskList);
/* Mergesort, NOTE: Order must strict */
void merge(Task *A, int mid, int high, bool (*order)(Task, Task));
void mergeSort(Task *A, int len, bool (*order)(Task, Task));
/* Input/Output */
void writeTaskList(List *taskList);
List* readTaskList(void);
void userInputLoop(List *taskList);
void shutdown(List *taskList);
void addTask(List *taskList);
void editTask(List *taskList);
void removeTask(List *taskList);
void newTasklist(List *taskList);
void displayTasklist(List *taskList);
/* List */
List* listInit(void);
Item* makeItem(Task data, Item *prev);
int cons(Task data, List *list);
int cdr(List *list);
Task car(List *list);
int freeList(List *list);
int emptyList(List *list);
Task* dumpToArray(List *list);

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

/* Wrapper function for Task struct */
Task
makeTask(char name[140], size_t v, time_t d, size_t t, size_t e)
{
	Task newTask;
	strcpy(newTask.name, name);
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

float
scoreFunc(Task task)
{
	/* The scoring formula, decides how to priorities tasks */
	return ((4*task.v_i+2*task.t_i+task.e_i)*3600)
		/(abs(abs(difftime(task.d,time(NULL)))-task.t)+0.1);
}

void
sortTasks(List *taskList)
{
	/* Init variables */
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
		A[i].v_i = i+1;
	}

	/* Get T index */
	mergeSort(A, len, sortT);
	for (int i = 0; i<len; ++i) {
		A[i].t_i = i+1;
	}
	
	/* Get E index */
	mergeSort(A, len, sortE);
	for (int i = 0; i<len; ++i) {
		A[i].e_i = i+1;
	}

	/* Calculate score and sort by score */
	for (int i = 0; i<len; ++i) {
		A[i].score = scoreFunc(A[i]);
	}
	mergeSort(A, len, sortScore);
	
	/* Empty taskList, avoid duplicates */
	emptyList(taskList);

	/* Dump sorted array */
	for (int i = 0; i<len; ++i) {
		cons(A[i],taskList);
	}

	/* No memory leaks allowed */
	free(A);
	return;
}

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
			 :                     A[m++]; /* h >= m, so we take m */
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

void
writeTaskList(List *taskList)
{
	/* Get array of tasks */
	Task *taskArray = dumpToArray(taskList);

	/* Open file */
	FILE *outfile;
	outfile = fopen("tasks.dat","wb+");

	/* Check if no errors */
	if (outfile == NULL) {
		printf("Error reading file\n");
		exit(1);
	}

	/* Write array to file */
	fwrite(taskArray, sizeof(Task), taskList->size, outfile);
	fclose(outfile);
	free(taskArray);
	return;
}


List*
readTaskList(void)
{
	/* Init file, tasklist and temp task */
	FILE *infile;	
	List *taskList = listInit();
	Task task;

	/* Open file */
	infile = fopen("tasks.dat","rb");

	/* Check if no errors */
	if (infile == NULL) {
		printf("Error reading file\n");
		exit(1);
	}

	/* Load into list */
	while(fread(&task, sizeof(Task), 1, infile)){
		cons(task,taskList);
	}

	/* Close and return */
	fclose(infile);
	return taskList;
}

void
userInputLoop(List *taskList)
{
	/* Get user input */
	int choice;
	printf("\n");
	printf("What do you want to do?\n");
	printf("1: Quit, 2: Add task, 3: Edit task\n");
	printf("4: Remove task, 5: Make new task list, 6: Display tasks\n");
	printf("Enter the number of what you wish to do: ");
	scanf("%d",&choice);

	/* Call the appropriate function, always tail call itself to
	 * make multiple input output actions possible. Except to quit */
	switch(choice) {
		case 1 :
			shutdown(taskList);
			break;
		case 2 :
			addTask(taskList);
			userInputLoop(taskList);
			break;
		case 3 :
			editTask(taskList);
			userInputLoop(taskList);
			break;
		case 4 :
			removeTask(taskList);
			userInputLoop(taskList);
			break;
		case 5 :
			emptyList(taskList);
			userInputLoop(taskList);
			break;
		case 6 :
			displayTasklist(taskList);
			userInputLoop(taskList);
			break;
		default :
			printf("Invalid input, try again.\n");
			userInputLoop(taskList);
	}

	return;
}

void
shutdown(List *taskList)
{
	/* Sort, write and frees list */
	sortTasks(taskList);
	writeTaskList(taskList);
	freeList(taskList);
	return;
}

void
addTask(List *taskList)
{
	/* Input variables*/
	char name[140]; 
	size_t V, E, T;
	int year, month, day;
	time_t D;
	struct tm *timeinfo;

	/* Input name */
	printf("Please enter the name of your task (max 140): ");
	scanf("%s",name);

	/* Input importance */
	printf("Please enter the (relative) importance of your task: ");
	scanf("%zu",&V);

	/* Input deadline */
	printf("Please enter the deadline of your task:\n");
	printf ("Enter year: "); scanf("%d",&year);
	printf ("Enter month: "); scanf("%d",&month);
	printf ("Enter day: "); scanf("%d",&day);
	time ( &D );
	timeinfo = localtime ( &D );
	timeinfo->tm_year = year - 1900;
	timeinfo->tm_mon = month - 1;
	timeinfo->tm_mday = day;
	D = mktime(timeinfo);

	/* Input energy */
	printf("Please enter the energy required by your task: ");
	scanf("%zu",&E);

	/* Input time */
	printf("Please enter the required time to complete your task in minutes: ");
	scanf("%zu",&T);
	T = 60*T;

	/* Make task and add to list */
	Task newTask = makeTask(name, V, D, T, E);
	cons(newTask,taskList);
	sortTasks(taskList);
    return; 
}

void
editTask(List *taskList)
{
	/* Variables */
	int size = taskList->size;
	int index;
	int choice;
	int year, month, day;
	time_t D;
	struct tm *timeinfo;
	Task *A = dumpToArray(taskList);

	/* Display list and let user choose element */
	displayTasklist(taskList);

	printf("Enter the number of the task: ");
	scanf("%d",&index);
	index = size-index;

	if (taskList->size-1 < index) {
		printf("Numbers to high\n");
		return;
	}

	/* Let user choose what to edit */
	printf("1: Name, 2: Importance, 3: Deadline, 4: Time, 5: Energy\n");
	printf("Enter the number of what you wish to edit: ");
	scanf("%d",&choice);

	/* Request appropriate input and change corresponding values
	 * Code lifted from makeTask */
	switch(choice) {
		case 1 :
    		printf("Enter name: ");
			scanf("%s",A[index].name);
			break;
		case 2 :
    		printf("Enter importance: ");
			scanf("%zu",&A[index].v);
			break;
		case 3 :
    		printf("Enter deadline: ");
			printf ("Enter year: "); scanf ("%d",&year);
			printf ("Enter month: "); scanf ("%d",&month);
			printf ("Enter day: "); scanf ("%d",&day);
			time ( &D );
			timeinfo = localtime ( &D );
			timeinfo->tm_year = year - 1900;
			timeinfo->tm_mon = month - 1;
			timeinfo->tm_mday = day;
			D = mktime(timeinfo);
			A[index].d	= D;
			break;
		case 4 :
    		printf("Enter time: ");
			scanf("%zu",&A[index].v);
			break;
		case 5 :
    		printf("Enter energy: ");
			scanf("%zu",&A[index].v);
			break;
		default :
			printf("Invalid choice\n");
	}

	emptyList(taskList);
	for (int i = 0; i<size; ++i) {
		cons(A[i],taskList);
	}

	sortTasks(taskList);
	return;
}

void
removeTask(List *taskList)
{
	/* Variables */
	int index;
	int size = taskList->size;
	Task *A = dumpToArray(taskList);

	/* Display list and request item to be removed */
	displayTasklist(taskList);

	printf("Enter the number of the task: ");
	scanf("%d",&index);
	index = size-index;

	if (taskList->size-1 < index) {
		printf("Numbers to high\n");
		return;
	}

	/* Empty task list and only copy over task if not at index */
	emptyList(taskList);
	for (int i = 0; i<size; i++) {
		if (i == index) {
			// Nothing
		} else {
			cons(A[i],taskList);
		}
	}

	sortTasks(taskList);
	free(A);
	return;
}

void
displayTasklist(List *taskList)
{
	/* Dump to temp array and loop through printing tasks */
	Task *A = dumpToArray(taskList);
	int size = taskList->size;
	for (int i = size-1; 0 <= i; i--) {
		printf("%d : ", size-i);
		printf("name: %s, ", A[i].name);
		printf("importance: %zu, ", A[i].v);
		printf("time: %zu, ", A[i].t);
		printf("energy: %zu, ", A[i].e);
		printf("date: %s ", asctime(localtime(&A[i].d)));
		printf("\n");
	}

	/* Free to avoid memory leaks */
	free(A);
	return;
}

/* Wrapper function for List struct */
List*
listInit(void)
{
	/* Allocates ram and sets temporary values */
	List *newList = (List*)malloc(sizeof(List));

	newList->top  = NULL;
	newList->size = 0;

	return newList;
}

/* Wrapper function for Item struct */
Item*
makeItem(Task data, Item *prev)
{
	/* Allocates ram and sets values */
	Item *newItem = (Item*)malloc(sizeof(Item));

	newItem->data = data;
	newItem->prev = prev;

	return newItem;
}

/* Add task to list top */
int
cons(Task data, List *list)
{
	Item *newItem = makeItem(data, list->top);
	list->top = newItem;
	++list->size;
	return 0;
}

/* Removes top item from list */
int
cdr(List *list)
{
	Item *oldTop = list->top;
	list->top = list->top->prev;	
	--list->size;
	free(oldTop);
	return 0;
}

/* Return top data */
Task
car(List *list)
{
	return list->top->data;
}

/* Free list from memory */
int
freeList(List *list)
{
	while (0 < list->size) {
		cdr(list);
	}

	free(list);
	return 0;
}

/* Empty list */
int
emptyList(List *list)
{
	while (0 < list->size) {
		cdr(list);
	}

	return 0;
}

/* write list to array for manipulations */
Task*
dumpToArray(List *list)
{
	/* Works by creating a local copy and copying to malloc'd array */
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

int
main(void)
{
	List *myTaskList = readTaskList();
	userInputLoop(myTaskList);

	return 1;
}
