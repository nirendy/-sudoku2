#ifndef MAPS_H
#define MAPS_H

#include "time.h"
#include "stdlib.h"


#define INVALID_VALUE -1;
#define INVALID_THRESHOLD -1;
#define MAX_STRING_LEN 1024

/* Typedefs */

typedef struct _gameDim {
    int n;
    int m;
    int N;
    int cellsCount;
    int cellNeighboursCount;
} GameDim;

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
    EFunctionFailed,
    EInvalidCommandInMode,
    /*parser errors*/
            EInvalidCommand,
    EInvalidNumOfParams,
    EInvalidFirstParam,
    EInvalidSecondParam,
    EInvalidThirdParam,
    EInvalidFile,
    ERedoUnavailable,
    EUndoUnavailable

} Error;

typedef enum _finish_code {
    FC_SUCCESS = 0,
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
    PValidateSuccess,
    PNextCommand,
    PNumSolutionsOutput,
    PPerformedChanges

} Prompt;

/*TODO: add more fields*/
typedef struct _game {
    Board solved_matrix;
    Board user_matrix;
    BoolBoard fixed_matrix;
    BoolBoard error_matrix;
    GameDim dim;
} Game;


typedef enum _command {
    COMMAND_SOLVE,
    COMMAND_EDIT,
    COMMAND_MARK_ERRORS,
    COMMAND_PRINT_BOARD,
    COMMAND_SET,
    COMMAND_VALIDATE,
    COMMAND_GUESS,
    COMMAND_GENERATE,
    COMMAND_UNDO,
    COMMAND_REDO,
    COMMAND_SAVE,
    COMMAND_HINT,
    COMMAND_GUESS_HINT,
    COMMAND_NUM_SOLUTIONS,
    COMMAND_AUTOFILL,
    COMMAND_RESET,
    COMMAND_EXIT,
    COMMAND_INVALID
} Command;


typedef struct _coordinate {
    int i;
    int j;
} Coordinate;


typedef struct _input {
    Command command;
    Coordinate coordinate;
    int value;
    int gen1;
    int gen2;
    float threshold;
    char path[MAX_STRING_LEN];
} Input;


typedef struct _DataNode {
    Bool isFirst;
    Input redoInput;
    Input undoInput;
    struct _DataNode* next;
    struct _DataNode* prev;
} DataNode;



/* A linked list node */
typedef struct _Node {
    Bool isFirst;
    DataNode* currDataNode;
    struct _Node* next;
    struct _Node* prev;
} Node;


/* Global variables */
GameDim gameDim;
Bool markError;
Node *firstNode;
Node *curNode;


void setGameDim(int n, int m);

Board createBoard();

void destroyBoard(Board board, GameDim dim);

Game *createGame();

void destroyGame(Game *game);

void copyBoard(Board targetBoard, Board copyFromBoard);

Coordinate createCoordinate(int i, int j);

void printError(Error err, Command command);

void printPrompt(Prompt prompt, int num1);

void askUserForNextTurn(Mode mode, Input *input);

void executeCommand(Input input, Mode *mode, Game **gameP);

FinishCode askUserForHintsAmount(int *hintsAmount);

int randLimit(int limit);

#endif

