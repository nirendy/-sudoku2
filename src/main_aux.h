#ifndef MAPS_H
#define MAPS_H

#include "time.h"
#include "stdlib.h"
int n;
int m;
typedef enum _bool {
    false = 0,
    true = 1
} Bool;

typedef int **Board;
typedef Bool **BoolBoard;

typedef enum _mode {
    Init,
    Solve,
    Edit,
    Exit
} Mode;

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
    PTitle,


    PEnterFixedAmount,
    PExit,
    PSuccess,
    PHint,
    PValidateFailed,
    PValidateSuccess

} Prompt;

/*TODO: add more fields*/
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
    COMMAND_EXIT,
    COMMAND_INVALID,
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
    INVALID
} Command;


typedef struct _input {
    Command command;
    Coordinate coordinate;
    int value;
} Input;


void printError(Error err, Command command);

void printPrompt(Prompt prompt, int num1);

FinishCode askUserForNextTurn(Mode mode, Input *input);

FinishCode executeCommand(Input input, Mode *mode, Game *game);

FinishCode askUserForDimension();

FinishCode askUserForHintsAmount(int *hintsAmount);

int randLimit(int limit);

#endif

