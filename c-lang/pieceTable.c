#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Piece {
	struct Piece *prev, *next; /* Previous/next piece in list */
	const char *data;          /* The data */
	size_t len;                /* Length of data */
	struct Piece *globalPrev;  /* Stack-like structure for freeing pieces */
	Text* text;                /* The text which the piece is in */
} Piece;

typedef struct Text {
	size_t len;       /* Length of text */
	Piece begin, end; /* Sentinel nodes containing no data */
	Piece* top;       /* Most recently added piece */
} Text;

typedef struct Location {
	size_t offset; /* Offset into to piece */
	Piece* piece;  /* The piece */
} Location;

Piece*
makePiece(Piece *prev, Piece *next, const char *data, Text* text)
{
	Piece *piece = (Piece*)malloc(sizeof(Piece));
	
	piece->prev = prev;
	piece->next = next;
	piece->len  = strlen(data);
	piece->globalPrev = text->top
	piece->text = text;
	
	return piece;
}

bool
pieceConc(Piece *piece1, Piece *piece2)
{
	piece1->next = 
}

Text*
textInit(void)
{
	Text *text = (Text*)malloc(sizeof(Text));
	
	text->top = NULL;
	text->len = 0;
	text->begin