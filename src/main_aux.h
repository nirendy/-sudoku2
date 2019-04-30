#ifndef MAIN_AUX_H
#define MAIN_AUX_H

#include "time.h"
#include "stdlib.h"


#define INVALID_VALUE -1;
#define INVALID_THRESHOLD -1;
#define MAX_PATH_LEN 1024
#define MAX_INPUT_STR_LEN 256


/* Main auxiliary module - responsible for general static functions
 *
 * setGameDim                     - Set game dimensions
 * setMode                        - Set game mode
 * setGlobalNodeNextToNull        - Set global node 'next' member to null
 * printError                     - Print error
 * printPrompt                    - Print prompt
 * smartMalloc                    - Smart malloc
 * smartCalloc                    - Smart calloc
 * randLimit                      - Get random integer between 0 to limit-1
 * removeArrayIndex               - Remove item from array
 * createBoard                    - Create board
 * destroyBoard                   - Destroy board
 * createGame                     - Create game
 * createGameFromFile             - Create game from file
 * destroyGame                    - Destroy game
 * terminateProgram               - Terminate program
 * createCoordinate               - Create coordinate
 * copyBoard                      - Copy board
 * getBoardValue                  - Get board value
 * setBoardValue                  - Set board value
 * getBoolBoardValue              - Get boolean board value
 * setBoolBoardValue              - Set boolean board value
 * isCoordinateFixed              - Is coordinate fixed
 * isCoordinateEmpty              - Is coordinate empty
 *
 * */

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
    ECellIsFixed,
    ECellIsNotEmpty,
    EInvalidCommand,
    ETooFewParams,
    ETooManyParams,
    EInvalidFirstParam,
    EInvalidSecondParam,
    EInvalidThirdParam,
    EInvalidFile,
    ERedoUnavailable,
    EUndoUnavailable,
    ENullNode,
    ENullDataNode,
    EInsertionInMiddle,
    EErroneousBoard,
    EFileSaveFailure,
    EFileOpenFailure,
    EFileCloseFailure,
    EFileScanFailure,
    EFUnsolvableBoard,
    EGenerationFailed,
    EInputTooLong,
    EGuessHintFailed,
    EGuessFailed,
    EMallocFailure,
    EReachUnexpectedEOF,
    EUnreachableCode
} Error;

typedef enum _prompt {
    PTitle,
    PExit,
    PSuccess,
    PHint,
    PValidateFailed,
    PValidateSuccess,
    PNextCommand,
    PNumSolutionsOutput,
    PPerformedChanges,
    PWrongSolution,
    PFileSavedSuccess,
    PNoObviousCells,
    PGuessFailure

} Prompt;

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
    char path[MAX_PATH_LEN];
} Input;

typedef struct _DataNode {
    Bool isFirst;
    Input redoInput;
    Input undoInput;
    struct _DataNode *next;
    struct _DataNode *prev;
} DataNode;

typedef struct _Node {
    Bool isFirst;
    DataNode *currDataNode;
    struct _Node *next;
    struct _Node *prev;
} Node;

/* Global variables */
/*
 * We chose to use 4 global variables
 * These variables are globally used and a lot of edge/small methods
 *   use them, and we wanted to avoid artificially passing them as arguments,
 *   what would have make the API much more complicated and less readable.
 * Furthermore, these variables are singletons of the program , and as mentioned are needed to be in a global scope.
 *
 * */
Mode g_mode;
GameDim g_gameDim;
Bool g_markError;
Node *g_curNode;

/* globals setters*/

void setGameDim(int n, int m);

void setMode(Mode newMode);

void setGlobalNodeNextToNull();

/* Prints */

void printError(Error err);

void printPrompt(Prompt prompt, int num1);

/* General Utilities*/

void *smartMalloc(size_t size);

void *smartCalloc(size_t numOfElements, size_t sizeOfElements);

int randLimit(int limit);

int removeArrayIndex(int *arr, int arrLength, int removeI);

/* Constructors and Destructors*/

Board createBoard();

void destroyBoard(Board board, GameDim dim);

Game *createGame();

Game *createGameFromFile(char *filePath);

void destroyGame(Game *game);

void terminateProgram(Game *game);

Coordinate createCoordinate(int i, int j);

/* Static Methods */

void copyBoard(Board targetBoard, Board copyFromBoard);

int getBoardValue(Board board, Coordinate coordinate);

void setBoardValue(Board board, Coordinate coordinate, int value);

Bool isCoordinateFixed(Game *game, Coordinate coordinate);

Bool isCoordinateEmpty(Game *game, Coordinate coordinate);


#endif

