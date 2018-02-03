#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Constants */
#define BEFORE 23
#define AFTER  24


/* Structs */
typedef struct Piece {
	int len;		    /* The length of the data */
	char* data;         /* Pointer into a buffer holding text */
	struct Piece* next; /* Pointer to the next/previous piece*/
	struct Piece* prev;
} Piece;

typedef struct Position {
	Piece* piece;
	int offset;
} Position;


/* Variables */
Piece* startPiece;
Piece* endPiece;
char* readBuffer = "This is a test";
char* appendBuffer;


/* Functions */
char* subString(char* buffer, int start, int stop);
Position getPosition(int index);
Piece* createPiece(char* data, Piece* next, Piece* prev);
int deletePiece(Piece* piece);
int connectPiece(Piece* piece1, Piece* piece2);
int splitPiece(Piece* piece, int offset);
int insert(int index, char* data);
int delete(int start, int end);


/* Function definitions */
/* Returns a segment of a buffer */
char*
subString(char* buffer, int start, int stop)
{
	int bufLen = strlen(buffer);

	if (stop > bufLen || start < 0) {
		return NULL;
	}

	char* dest = (char*)malloc((stop-start+1)*sizeof(char));
	strncpy(dest, buffer+start, stop-start);
	return dest;
}

/* Get the position by index */
Position
getPosition(int index)
{
	size_t cur = 0;
	for (Piece *p = startPiece; p->next; p = p->next) {
		if (cur <= index && index <= cur + p->len) {
			return (Position){.piece = p, .offset = index-cur}; 
		} else {
			cur += p->len;
		}
	}
	/* Return nothing */
	return (Position){ 0 };
}

/* Creates a piece using the specified values. */
Piece*
createPiece(char* data, Piece* next, Piece* prev)
{
	Piece* newPiece = NULL;
	newPiece = (Piece *)malloc(sizeof(Piece));
	
	newPiece->data = data;
	newPiece->next = next;
	newPiece->prev = prev;
	newPiece->len  = strlen(data);
	
	
	return newPiece;
}

/* Delete a piece */
int
deletePiece(Piece* piece)
{
	if (piece == NULL) {
		return 1;
	} else {
	connectPiece(piece->prev, piece->next);
	free(piece->data);
	free(piece);
	
	return 0;
	}
}

/* Connect two pieces, piece 1 first */
int
connectPiece(Piece* piece1, Piece* piece2)
{
	piece1->next = piece2;
	piece2->prev = piece1;
	
	return 0;
}

/* Split a piece at a given offset into two pieces */
int
splitPiece(Piece* piece, int offset)
{
	char* pieceData = piece->data;
	int pieceLength       = strlen(pieceData);
	
	/* Check for offset at boundary and wrong offset */
	if ( 0 >= offset ) {
		return BEFORE;
	} else if ( pieceLength <= offset ) {
		return AFTER;
	}
	
	/* Split the data */
	char* piece1Data = subString(pieceData, 0, offset);
	char* piece2Data = subString(pieceData, offset+1, pieceLength);
	
	/* Init the two pieces */
	Piece* piece1 = createPiece(piece1Data, NULL, NULL);
	Piece* piece2 = createPiece(piece2Data, NULL, NULL);
	
	/* Check if creation went well */
	if ( NULL == piece1 || NULL == piece2 )
		return 1;
	
	/* Connect it all up */
	connectPiece(piece->prev, piece1);
	connectPiece(piece1, piece2);
	connectPiece(piece2, piece->next);

	/* Free the piece */
	free(piece->data);
	free(piece);
	
	return 0;
}

/* Insert a string at index */
int
insert(int index, char* data)
{
	/* Get the necessary information to insert a piece */
	Piece* newPiece = createPiece(data, NULL, NULL);
	Position position = getPosition(index);
	Piece* prevPiece   = position.piece->prev;
	Piece* nextPiece   = position.piece->next;
	int insertLocation = splitPiece(position.piece, position.offset);
	
	if (AFTER == insertLocation) {
		/* Inserting after a piece, therefore we jump over it */
		connectPiece(prevPiece->next, newPiece);
		connectPiece(newPiece, nextPiece);
		
	} else if (BEFORE == insertLocation) {
		/* Inserting before a piece, therefore we jump back */
		connectPiece(prevPiece, newPiece);
		connectPiece(newPiece, nextPiece->prev);
		
	} else {
		/* Inserting in the middle of piece, therefore jump into it */
		connectPiece(prevPiece->next, newPiece);
		connectPiece(newPiece, nextPiece->prev);
	}
	
	return 0;
}

/* Delete a sequence */
int
delete(int start, int end)
{
	Position startPosition = getPosition(start);
	Position endPosition   = getPosition(end);
	
	splitPiece(startPosition.piece, startPosition.offset);
	splitPiece(endPosition.piece, startPosition.offset);
	
	Position currentPosition;
	
	while (start < end) {
		currentPosition = getPosition(start);
		end = end - currentPosition.piece->len;
		deletePiece(currentPosition.piece);
	}
	
	return 0;
}


int
main(void)
{
	startPiece = createPiece("start ", NULL, NULL);
	endPiece = createPiece("end", NULL, NULL);
	connectPiece(startPiece, endPiece);
	Position pos = getPosition(7);
//	Piece* POS = pos.piece;
	/* Cant access the piece data from position */
//	printf("%c\n", *(POS->data));
}
