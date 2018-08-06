#include <stdio.h>
#include <time.h>

#define MAX_HEIGHT 20
#include "skip_list.h"

int
main(void)
{
	srand(time(NULL) + rand());
	Skip_list *skip_list = skip_list_new(NULL);
	int len = 1000000;
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
		skip_list_insert(skip_list,&data[i],keys[i]);
	}

	for (int i = 0; i < 100; i++) {
		int search_data = *(int*)skip_list_search(skip_list,keys[i])->data;

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
			int search_data = *(int*)skip_list_search(skip_list,keys[i])->data;

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
			skip_list_insert(skip_list,&data[rand() % 101],(rand() % len) + 1);
		}
	}

	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

	printf("Time %f\n", time_taken);

	skip_list_free(skip_list);

	return 0;
}
