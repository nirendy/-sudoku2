#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main_aux.h"
#include "solver.h"
#include "parser.h"
#include "file_handler.h"
#include "linked_list.h"

/* globals setters*/

void setGameDim(int n, int m) {
    g_gameDim.n = n;
    g_gameDim.m = m;
    g_gameDim.N = n * m;
    g_gameDim.cellsCount = g_gameDim.N * g_gameDim.N;
    g_gameDim.cellNeighboursCount = 3 * g_gameDim.N - n - m - 1;
}

void setMode(Mode newMode) {
    g_mode = newMode;
}

void setGlobalNodeNextToNull() {
    g_curNode->next = NULL;
}


/* Prints */

void printError(Error err) {

    switch (err) {
        case ECellIsFixed: {
            printf("Error: cell is fixed\n");
            break;
        }
        case ECellIsNotEmpty: {
            printf("Error: the cell already contains a value\n");
            break;
        }
        case EInvalidCommand: {
            printf("Error: invalid command\n");
            break;
        }
        case ETooFewParams: {
            printf("Error: not enough parameters were entered\n");
            break;
        }
        case ETooManyParams: {
            printf("Error: too many parameters were entered\n");
            break;
        }
        case EInvalidFile: {
            printf("Error: text in file is invalid\n");
            break;
        }
        case EInvalidFirstParam: {
            printf("Error: first parameter is invalid\n");
            break;
        }
        case EInvalidSecondParam: {
            printf("Error: second parameter is invalid\n");
            break;
        }
        case EInvalidThirdParam: {
            printf("Error: third parameter is invalid\n");
            break;
        }
        case ERedoUnavailable: {
            printf("Error: Redo command is not possible\n");
            break;
        }
        case EUndoUnavailable: {
            printf("Error: Undo command is not possible\n");
            break;
        }
        case EErroneousBoard: {
            printf("Error: Board is erroneous\n");
            break;
        }
        case EFileSaveFailure: {
            printf("Error: File save failed\n");
            break;
        }
        case EFileOpenFailure: {
            printf("Error: File open failed\n");
            break;
        }
        case EFileCloseFailure: {
            printf("Error: File close failed\n");
            break;
        }
        case EFileScanFailure: {
            printf("Error: Game dimensions scan failed\n");
            break;
        }
        case EFUnsolvableBoard : {
            printf("Error: board is unsolvable\n");
            break;
        }
        case EGenerationFailed : {
            printf("Error: couldn't generate a solved board after 1000 iterations\n");
            break;
        }
        case EInputTooLong : {
            printf("Error: too many characters where entered in a single line\n");
            break;
        }
        case EGuessHintFailed: {
            printf("Error: Could not guess hint\n");
            break;
        }
        case EMallocFailure : {
            printf("Error: Memory allocation failed - terminating program\n");
            break;
        }
        case EReachUnexpectedEOF : {
            printf("Error: Reached unexpected EOF - terminating program\n");
            break;
        }
        case ENullNode: {
            printf("Error: Null node given as a pointer - unreachable code\n");
            break;
        }
        case ENullDataNode: {
            printf("Error: Null Data node given as a pointer - unreachable code\n");
            break;
        }
        case EInsertionInMiddle: {
            printf("Error: Insertion in the middle of the list - unreachable code\n");
            break;
        }
        case EUnreachableCode: {
            printf("Unreachable Code Error - terminating...\n");
            exit(-1);
        }
        default: {
            printError(EUnreachableCode);
        }
    }
}

void printPrompt(Prompt prompt, int num1) {
    switch (prompt) {

        case PTitle: {
            printf("Sudoku Game\n");
            break;
        }
        case PExit: {
            printf("Exiting...\n");
            break;
        }
        case PSuccess: {
            printf("Puzzle solved successfully - setting mode back to Init\n");
            break;
        }
        case PHint: {
            printf("Hint: set cell to %d\n", num1);
            break;
        }
        case PValidateFailed: {
            printf("Validation failed: board is unsolvable\n");
            break;
        }
        case PValidateSuccess: {
            printf("Validation passed: board is solvable\n");
            break;
        }
        case PNextCommand: {
            printf("Please enter the desired command:\n");
            break;
        }
        case PNumSolutionsOutput: {
            printf("There are: %d possible solutions\n", num1);
            break;
        }
        case PPerformedChanges: {
            printf("The performed changes are:\n");
            break;
        }
        case PNoObviousCells: {
            printf("The were no obvious cells to fill:\n");
            break;
        }
        case PGuessFailure: {
            printf("Couldn't guess any cells:\n");
            break;
        }
        case PWrongSolution: {
            printf("The solution to the board is wrong\n");
            break;
        }
        case PFileSavedSuccess: {
            printf("The file was saved successfully\n");
            break;
        }
        default: {
            printf("Unreachable Code Error");
        }
    }
}

/* General Utilities*/

/**
 * Allen's brilliant idea to switch all mallocs with a function that checks whether or not the malloc failed
 * if it did, it exit with a proper error message else, it returns the pointer as the original malloc would do
 * @param size
 * @return pointer to the created resource
 */
void *smartMalloc(size_t size) {
    void *p;
    if (size == 0) {
        return NULL;
    }

    p = malloc(size);
    if (p == NULL) {
        printError(EMallocFailure);
        exit(-1);
    }

    return p;
}
/**
 * Nir brilliant idea of copying Allen's brilliant idea to Calloc as well.
 * See smartMalloc
 * @param numOfElements
 * @param sizeOfElements
 * @return pointer to the created resource
 */
void *smartCalloc(size_t numOfElements, size_t sizeOfElements) {

    void *p;
    if (numOfElements * sizeOfElements == 0) {
        return NULL;
    }

    p = calloc(numOfElements, sizeOfElements);
    if (p == NULL) {
        printError(EMallocFailure);
        exit(-1);
    }

    return p;
}

int randLimit(int limit) {
    return rand() % limit;
}

int removeArrayIndex(int *arr, int arrLength, int removeI) {
    /* return the removed value */

    int i, removedValue = arr[removeI];

    /*closing the gap of the removed index*/
    for (i = removeI; i < arrLength - 1; ++i) {
        arr[i] = arr[i + 1];
    }

    return removedValue;
}

/* Constructors and Destructors*/

Board createBoard() {
    Board board = (int **) smartMalloc(g_gameDim.N * sizeof(int *));
    int i;

    for (i = 0; i < g_gameDim.N; ++i) {
        board[i] = (int *) smartMalloc(g_gameDim.N * sizeof(int));
    }

    return board;
}

void destroyBoard(Board board, GameDim dim) {
    {
        int i;
        for (i = 0; i < dim.N; ++i) {
            free(board[i]);
        }
    }

    free(board);
}

BoolBoard createBoolBoard() {
    BoolBoard board = (Bool **) smartMalloc(g_gameDim.N * sizeof(Bool *));
    int i;

    for (i = 0; i < g_gameDim.N; ++i) {
        board[i] = (Bool *) smartMalloc(g_gameDim.N * sizeof(Bool));
    }

    return board;
}

void destroyBoolBoard(BoolBoard board, GameDim dim) {
    {
        int i;
        for (i = 0; i < dim.N; ++i) {
            free(board[i]);
        }
    }

    free(board);
}

Game *createGame() {
    Game *game = smartMalloc(sizeof(Game));
    game->dim = g_gameDim;
    game->solved_matrix = createBoard();
    game->user_matrix = createBoard();
    game->fixed_matrix = createBoolBoard();
    game->error_matrix = createBoolBoard();

    return game;
}

Game *createGameFromFile(char *filePath) {
    Game *newGame;
    GameDim oldDimensions = g_gameDim;

    if (setDimensionsFromFile(filePath) == false) {
        setGameDim(oldDimensions.n, oldDimensions.m);
        return NULL;
    }

    newGame = createGame();

    if (generateGameFromFile(filePath, newGame) == false) {
        destroyGame(newGame);
        setGameDim(oldDimensions.n, oldDimensions.m);
        return NULL;
    }

    return newGame;
}

void destroyGame(Game *game) {
    destroyBoard(game->solved_matrix, game->dim);
    destroyBoard(game->user_matrix, game->dim);
    destroyBoolBoard(game->fixed_matrix, game->dim);
    destroyBoolBoard(game->error_matrix, game->dim);

    free(game);
}

/**
 * frees all resources gracefully, prints a message and exits
 * @param game
 */
void terminateProgram(Game *game) {
    if (game != NULL) {
        destroyGame(game);
    }
    clearListFromNode(getFirstNode(g_curNode));
    printPrompt(PExit, 0);

    exit(0);
}

Coordinate createCoordinate(int i, int j) {
    Coordinate result;
    result.i = i;
    result.j = j;
    return result;
}

/* Static Methods */

void copyBoard(Board targetBoard, Board copyFromBoard) {
    int i, j;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            targetBoard[i][j] = copyFromBoard[i][j];
        }
    }

}

int getBoardValue(Board board, Coordinate coordinate) {
    return board[coordinate.i][coordinate.j];
}

void setBoardValue(Board board, Coordinate coordinate, int value) {
    board[coordinate.i][coordinate.j] = value;
}

Bool isCoordinateFixed(Game *game, Coordinate coordinate) {
    return game->fixed_matrix[coordinate.i][coordinate.j] == true;
}

Bool isCoordinateEmpty(Game *game, Coordinate coordinate) {
    return game->user_matrix[coordinate.i][coordinate.j] == 0;
}
