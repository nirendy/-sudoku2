#ifndef MAPS_H
#define MAPS_H

#include "time.h"
#include "stdlib.h"

#define N 3 /* block height*/
#define M 3 /* block width*/

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

typedef enum _finish_code {
    FC_SUCCESS=0,
    FC_EOF,
    FC_INVALID_RECOVERABLE,
    FC_INVALID_TERMINATE,
    FC_UNEXPECTED_ERROR
} FinishCode;

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
    COMMAND_SET,
    COMMAND_HINT,
    COMMAND_VALIDATE,
    COMMAND_RESTART,
    COMMAND_EXIT,
    COMMAND_INVALID
} Command;

typedef struct _input {
    Command command;
    Coordinate coordinate;
    int value;
} Input;


void printError(Error err, Command command);

void printPrompt(Prompt prompt, int num1);

FinishCode askUserForNextTurn(Input *input);

FinishCode askUserForHintsAmount(int *hintsAmount);

int randLimit(int limit);

#endif

