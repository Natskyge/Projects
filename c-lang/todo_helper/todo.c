#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <editline/readline.h>

#define MAX_HEIGHT 10 /* Enough to handle 2^10 items */
#include "skip_list.h"
#include "task.h"


/* TODO: The printed data is garbage. */

void user_shutdown(Skip_list *skip_list, char *name);
void user_task_add(Skip_list *skip_list);
void user_task_edit(Skip_list *skip_list);
void user_task_remove(Skip_list *skip_list);
void user_list_clear(Skip_list *skip_list);
void user_list_display(Skip_list *skip_list);

float
sig(float x)
{
	/* Sends R to [0,10] */
	float y = (x-180)/10;
	float s = y/(1+fabs(y));
	return 5*(s+1);
}

float
task_score(Task task)
{
	return 100000*(4*task.importance + 2*sig(task.time_) + task.energy)
		   /(fabs(difftime(task.deadline,time(NULL))/3600 - 60*task.time_)+0.1);
}

Task
task_new(char name[140], size_t importance, size_t time_, size_t energy, time_t deadline)
{
	Task new_task;

	strcpy(new_task.name, name);
	new_task.importance = importance;
	new_task.time_ = time_;
	new_task.energy = energy;
	new_task.deadline = deadline;
	new_task.score = task_score(new_task);

	return new_task;
}

Task*
task_to_array(Skip_list *skip_list)
{
	size_t len = skip_list->len;
	Node *node = skip_list->header->shortcuts[0];
	Task *array = malloc(len*sizeof(Task));

	for (int i = 0; i<len; i++) {
		array[len-i-1] = node->data;
		node = node->shortcuts[0];
	}

	return array;
}

void
skip_list_write(Skip_list *skip_list, const char *name)
{
	if (!skip_list) {
		/* Nothing to do */
		return;
	}

	if (!name) {
		printf("Error: skip_list_write() null name");
		exit(EXIT_FAILURE);
	}

	Task *array = task_to_array(skip_list);

	FILE *out_file;
	out_file  = fopen(name,"wb+");

	if (!out_file) {
		printf("Error reading file: %s\n", name);
		exit(EXIT_FAILURE);
	}

	fwrite(array,sizeof(Task), skip_list->len, out_file);
	fclose(out_file);
	free(array);
	return;
}

Skip_list*
skip_list_read(const char *name)
{
	FILE *in_file;
	Skip_list *skip_list = skip_list_new(NULL);
	Task array[2048]; /* Probaly big enough */
	int i = 0;
	Task task;

	in_file = fopen(name,"ab+");

	if (!in_file) {
		printf("Error reading file: %s\n", name);
		exit(EXIT_FAILURE);
	}

	while(fread(&task, sizeof(Task), 1, in_file)) {
		array[i] = task;
		skip_list_insert(skip_list,array[i],array[i].score);
		++i;
	}

	return skip_list;
}

void
user_input_loop(Skip_list *skip_list, char *name)
{
	printf("[q]uit, [a]dd task, [e]dit task, [r]emove task, [c]lear list");
	printf(", [d]isplay tasks\n");
	char* input = readline("Choice >>> ");
	add_history(input);
	char choice  = input[0];

	switch(choice) {
		case 'q' :
			user_shutdown(skip_list,name);
			break;
		case 'a' :
			user_task_add(skip_list);
			user_input_loop(skip_list,name);
			break;
		case 'e' :
			user_task_edit(skip_list);
			user_input_loop(skip_list,name);
			break;
		case 'r' :
			user_task_remove(skip_list);
			user_input_loop(skip_list,name);
			break;
		case 'c' :
			user_list_clear(skip_list);
			user_input_loop(skip_list,name);
			break;
		case 'd' :
			user_list_display(skip_list);
			user_input_loop(skip_list,name);
			break;
		default :
			printf("Invalid input, try again.\n");
			user_input_loop(skip_list,name);
	}

	free(input);
}

void
user_shutdown(Skip_list *skip_list, char *name)
{
	skip_list_write(skip_list,name);
	skip_list_free(skip_list);
}

void
user_task_add(Skip_list *skip_list)
{
	char name[140];
	size_t importance, energy, time_;
	int year, month, day;
	time_t deadline;
	struct tm *timeinfo;
	char *input;
	char *end;

	input = readline("Name [max length 140] >>> ");
	add_history(input);
	strcpy(name,input);

	input = readline("Importance [0/10] >>> ");
	add_history(input);
	importance = strtol(input,&end,10);

	printf("Deadline of your task:\n");
	input = readline("Year  >>> ");
	add_history(input);
	year = strtol(input,&end,10);
	input = readline("Month >>> ");
	add_history(input);
	month = strtol(input,&end,10);
	input = readline("Day   >>> ");
	add_history(input);
	day = strtol(input,&end,10);
	time(&deadline);
	timeinfo = localtime(&deadline);
	timeinfo->tm_year = year - 1900;
	timeinfo->tm_mon  = month - 1;
	timeinfo->tm_mday = day;
	deadline = mktime(timeinfo);

	input = readline("Energy [0/10] >>> ");
	add_history(input);
	energy = strtol(input,&end,10);

	input = readline("Time [min] >>> ");
	add_history(input);
	time_ = strtol(input,&end,10);

	Task task = task_new(name,importance,time_,energy,deadline);
	skip_list_insert(skip_list,task,task.score);

	free(input);
}

void
user_task_edit(Skip_list *skip_list)
{
	char *input;
	char *end;
	size_t index;
	int year, month, day;
	time_t deadline;
	struct tm *timeinfo;

	user_list_display(skip_list);
	input = readline("Number of the task >>> ");
	add_history(input);

	index = strtol(input,&end,10);

	if (skip_list->len < index) {
		printf("Number to high, exiting...\n");
		return;
	}

	Task *tasks = task_to_array(skip_list);

	Task task = tasks[index-1];


	printf("[n]ame, [i]mportanc, [t]ime, [e]nergy, [d]eadline\n");
	input = readline("Edit >>> ");
	add_history(input);
	char choice  = input[0];

	switch(choice) {
		case 'n' :
			input = readline("Name [max length 140] >>> ");
			add_history(input);
			strcpy(task.name,input);
			break;
		case 'i' :
			input = readline("Importance [0/10] >>> ");
			add_history(input);
			task.importance = strtol(input,&end,10);
			break;
		case 't' :
			input = readline("Time [min] >>> ");
			add_history(input);
			task.time_ = strtol(input,&end,10);
			break;
		case 'e' :
			input = readline("Energy [0/10] >>> ");
			add_history(input);
			task.energy = strtol(input,&end,10);
			break;
		case 'd' :
			printf("Deadline of your task:\n");
			input = readline("Year  >>> ");
			add_history(input);
			year = strtol(input,&end,10);
			input = readline("Month >>> ");
			add_history(input);
			month = strtol(input,&end,10);
			input = readline("Day   >>> ");
			add_history(input);
			day = strtol(input,&end,10);
			time(&deadline);
			timeinfo = localtime(&deadline);
			timeinfo->tm_year = year - 1900;
			timeinfo->tm_mon  = month - 1;
			timeinfo->tm_mday = day;
			task.deadline = mktime(timeinfo);
			break;
		default :
			printf("No valid option chosen.");
	}

	/* Update the node position */
	skip_list_delete(skip_list,task.score);
	task.score = task_score(task);
	skip_list_insert(skip_list,task,task.score);

	free(input);
}

void
user_task_remove(Skip_list *skip_list)
{
	int index;
	int score;
	char *input;
	char *end;

	user_list_display(skip_list);
	input = readline("Number of the task >>> ");
	add_history(input);

	index = strtol(input,&end,10);

	if (skip_list->len < index) {
		printf("Number to high, exiting...\n");
		return;
	}

	Task *tasks = task_to_array(skip_list);

	score = tasks[index-1].score;
	skip_list_delete(skip_list,score);

	free(input);
}

void
user_list_clear(Skip_list *skip_list)
{
	int cur_key;
	if (!skip_list) {
		return;
	}

	while (skip_list->header->shortcuts[0]->key != INT_MAX) {
		cur_key = skip_list->header->shortcuts[0]->key;
		skip_list_delete(skip_list,cur_key);
	}
}

void
user_list_display(Skip_list *skip_list)
{
	if (skip_list->len == 0) {
		return;
	}
	Task *tasks = task_to_array(skip_list);
	for (size_t i = 0; i < skip_list->len; ++i) {
		printf("%zu : ", i+1);
		printf("name: %s, ", tasks[i].name);
		printf("importance: %zu, ", tasks[i].importance);
		printf("time: %zu, ", tasks[i].time_);
		printf("energy: %zu, ", tasks[i].energy);
		printf("deadline: %s\n", asctime(localtime(&tasks[i].deadline)));
	}
}

int
main(int argc, char **argv)
{
	if (argc == 2) {
		Skip_list *skip_list = skip_list_read(argv[1]);
		user_input_loop(skip_list,argv[1]);
	} else {
		printf("Error: Too many or too few arguments.");
		exit(EXIT_FAILURE);
	}

	return 0;
}
