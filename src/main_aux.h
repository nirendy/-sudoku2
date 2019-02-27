#ifndef MAPS_H
#define MAPS_H

#include "time.h"
#include "stdlib.h"

//#define N 3 /* block height*/
//#define M 3 /* block width*/

typedef enum _bool {
    false = 0,
    true = 1
} Bool;

typedef int **Board;
typedef Bool **BoolBoard;

typedef enum _error {
    EInvalidNumberOfCells,
    ECellIsFixed,
    EValueIsInvalid,
    EInvalidCommand,
    EFunctionFailed
} Error;

typedef enum _prompt {
    PEnterFixedAmount,
    PExit,
    PSuccess,
    PHint,
    PValidateFailed,
    PValidateSuccess

} Prompt;

typedef struct _game {
    Board solved_matrix;
    Board user_matrix;
    BoolBoard fixed_matrix;
} Game;

Game *createGame();

void destroyGame(Game *game);


typedef struct _coordinate {
    int i;
    int j;
} Coordinate;

Coordinate createCoordinate(int i, int j);

typedef enum _command {
	SOLVE,
	EDIT,
	MARK_ERRORS,
	PRINT_BOARD,
	SET,
	VALIDATE,
	GUESS,
	GENERATE,
	UNDO,
	REDO,
	SAVE,
	HINT,
	GUESS_HINT,
	NUM_SOLUTIONS,
	AUTOFILL,
	RESET,
	EXIT,
	INVALID,

} Command;

typedef struct _input {
    Command command;
    Coordinate coordinate;
    int value;
} Input;


void printError(Error err, Command command);

void printPrompt(Prompt prompt, int num1);

Input askUserForNextTurn();

int askUserForHintsAmount();

int randLimit(int limit);

#endif

