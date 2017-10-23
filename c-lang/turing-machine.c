/*            *----------------------------------------------*
 *            | Turing machine program, by Erik. S. Gimsing. |
 *            *----------------------------------------------*
 *
 * It is currently set up as a 3-state busy beaver, but it should possible to
 * modify it to any other by changing the tape and the selection tabel.
 * Symbols are represented as numbers, in this case its 0 and 1.
 * State is also indicated by numbers with state 0 causing halt.
 * The turing machine reads the symbol and state, and then uses the tabel to
 * choose what to write, the next state and where to move.
 */

/* Header files */
#include <stdio.h>
#include <stdlib.h>

/* Functions */
static int  Mod(int a, int b);
static int  PrintArray(int array[], int length);
static void TuringMachine(int state, int position, int tape[], int length);

/* Constants */
#define TapeLength   32
#define SymbolAmount 2
#define StateAmount  3

/* Variables */
static int tape[TapeLength];
static int outputs[SymbolAmount][StateAmount] = {
    {1, 1, 1},
    {1, 1, 1}};
static int states[SymbolAmount][StateAmount] = {
    {2, 1, 2},
    {3, 2, 0}};
static int moves[SymbolAmount][StateAmount] = {
    { 1, -1, -1},
    {-1,  1,  1}};

/* Function defintions */
void
TuringMachine(int state, int position, int tape[], int length)
{
    /* Halt when state is 0 */
    if (state == 0) {
    } else {

    /* Read symbol for later use, write to tape and choose next position */
    int read_symbol  = tape[position];
    tape[position]   = outputs[read_symbol][state];
    int move         = moves[read_symbol][state];
    int new_position = Mod(position + move, length);
    int new_state    = states[read_symbol][state];

    /* Move to the next position */
    PrintArray(tape, TapeLength);
    TuringMachine(new_state, new_position, tape, length);
    }
}

int
Mod(int a, int b)
{
    if(b < 0)
        return Mod(a, -b);
    int ret = a % b;
    if(ret < 0)
        ret+=b;
    return ret;
}

int
PrintArray(int array[], int length)
{
    for(int n = 0; n < length; n++) {
        printf("%d", array[n]);
    }
    printf("\n");
    return 0;
}

/* main */
int
main(void)
{
    /* Initialize tape */
    for (int i; i < TapeLength; i++) {
        tape[i] = 0;
    }

    /* Print tape, call TuringMachine and print result */
    PrintArray(tape, TapeLength);
    TuringMachine(1, 8, tape, TapeLength);
    PrintArray(tape, TapeLength);
}
