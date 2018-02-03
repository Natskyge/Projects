#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 10
#define WIDTH  8
#define WALL   0
#define PATH   1
#define EMPTY  2

typedef struct point {
	int x;
	int y;
} Point;

int maze[WIDTH][HEIGHT];

int printMaze(int maze[]);
int mod(int a, int n);
int RANDOM(int max);
int initMaze(void);

Point
newDirection(int dir)
{
	Point newPoint;
	switch(dir) {
		case 0:
			newPoint.x =  0; 
			newPoint.y =  1;
		case 1:
			newPoint.x =  1;
			newPoint.y =  0;
		case 2:
			newPoint.x =  0;
			newPoint.y = -1;
		case 3:
			newPoint.x = -1;
			newPoint.y =  0;
		default:
			newPoint.x =  0;
			newPoint.y =  0;
	}

	return newPoint;
}

int
mazeGenerator(int x, int y, int TYPE)
{
	if (maze[x][y] != EMPTY)
		return 0;

	maze[x][y] = TYPE;

	int type[numberOfConections];

	for (int i = 0; i <= numberOfConections; i++) {
		type[i] = RANDOM(1);
	}

	for (int dir = 0; dir <= 3; dir++) {
		Point goTo = newDirection(dir);
		mazeGenerator(mod(goTo.x, WIDTH), mod(goTo.y, HEIGHT), type[dir]);
	}

	return 0;
}

int
main(void)
{
	return 0;
}
