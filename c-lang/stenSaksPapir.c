#include <stdio.h>
#include <stdlib.h>

#define PAPIR 0
#define SAKS  1
#define STEN  2

char*
whoWon(int player1Input, int player2Input)
{
	int diff = player1Input - player2Input;
	if (diff == 0) {
		return "Draw\n";
	} else if (diff == 1 || diff == -2) {
		return "Player 1 wins!\n";
	} else {
		return "Player 2 wins!\n";
	}
}

int
main(void)
{
	printf(whoWon(SAKS,  PAPIR));
	printf(whoWon(PAPIR, SAKS));
	printf(whoWon(PAPIR, STEN));
	printf(whoWon(STEN,  PAPIR));
	printf(whoWon(STEN,  SAKS));
	printf(whoWon(SAKS,  STEN));
	return 1;
}
