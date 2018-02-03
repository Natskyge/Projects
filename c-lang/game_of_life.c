#include <stdio.h>
#include <stdlib.h>

#define WIDTH  10
#define HEIGHT 13

int nCount[WIDTH][HEIGHT];
int board[WIDTH][HEIGHT];

int
printArray(int array[WIDTH][HEIGHT])
{
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			printf("%d", array[x][y]);
		}
		printf("\n");
	}
	return 0;
}

int
alive(int x, int y)
{
	if (x >= WIDTH || y >= HEIGHT) {
		return 0;
	} else {
		int n = board[x][y];
		return n % 2;
	}
}

int
getNeighbours(void)
{
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			nCount[x][y] = 0;
		}
	}

	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			nCount[x][y]     += alive(x+1,y)+alive(x,y+1)+alive(x+1,y+1);
			nCount[x+1][y]   += alive(x,y+1)+alive(x,y);
			nCount[x][y+1]   += alive(x+1,y)+alive(x,y);
			nCount[x+1][y+1] += alive(x,y);
		}
	}
	
	return 0;
}

int
main(void)
{
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			board[x][y] = rand() % 2;
		}
	}

	printArray(board);
	getNeighbours();
	printf("\n");
	printArray(nCount);
	printf("\n");
	printArray(board);
	
	return 0;
}
