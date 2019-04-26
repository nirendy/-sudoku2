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

/* Prints */

void printError(Error err) {

    switch (err) {
        case EInvalidNumberOfCells:
            printf("Error: invalid number of cells to fill (should be between 0 and %d)\n", g_gameDim.cellsCount - 1);
            break;
        case ECellIsFixed: {
            printf("Error: cell is fixed\n");
            break;
        }
        case ECellIsNotEmpty: {
            printf("Error: cell is not empty\n");
            break;
        }
        case EValueIsInvalid: {
            printf("Error: value is invalid\n");
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
        case EErroneousBoard: {
            printf("Error: Board is erroneous\n");
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
            printf("Error: Scan failed\n");
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
        case EMallocFailure : {
            printf("Error: Memory allocation failed - terminating program\n");
            break;
        }
        default: {
            printf("Unreachable Code Error\n");
        }
    }
}

void printPrompt(Prompt prompt, int num1) {
    switch (prompt) {
        case PEnterFixedAmount: {
            printf("Please enter the number of cells to fill [0-%d]:\n", g_gameDim.cellsCount - 1);
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
        case PTitle: {
            printf("Sudoku Game\n");
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
        case PWrongSolution: {
            printf("The solution to the board is wrong:\n");
            break;
        }
        case PNoObviousCells: {
            printf("There were no obvious cells to autofill\n");
            break;
        }
        default: {
            printf("Unreachable Code Error");
        }
    }
}

/* General Utilities*/

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
    GameDim oldDimensions = g_gameDim;
    FinishCode finishCode = setDimensionsFromFile(filePath);

    if (finishCode == FC_INVALID_RECOVERABLE) {
        setGameDim(oldDimensions.n, oldDimensions.m);
        return NULL;
    } else if (finishCode == FC_SUCCESS) {
        Game *newGame = createGame();
        finishCode = generateGameFromFile(filePath, newGame);

        if (finishCode == FC_INVALID_RECOVERABLE) {
            destroyGame(newGame);
            setGameDim(oldDimensions.n, oldDimensions.m);
            return NULL;
        } else if (finishCode == FC_SUCCESS) {
            return newGame;
        }
    }

    exit(finishCode);
}

void destroyGame(Game *game) {
    destroyBoard(game->solved_matrix, game->dim);
    destroyBoard(game->user_matrix, game->dim);
    destroyBoolBoard(game->fixed_matrix, game->dim);
    destroyBoolBoard(game->error_matrix, game->dim);

    free(game);
}

void terminateProgram(Game *game, FinishCode finishCode) {
    if (game != NULL) {
        destroyGame(game);
    }
    clearListFromNode(getFirstNode(g_curNode));
    printPrompt(PExit, COMMAND_INVALID);

    if (finishCode == FC_UNEXPECTED_ERROR || finishCode == FC_INVALID_TERMINATE) {
        exit(-1);
    }
    exit(0);
}

/* Static Methods */

Coordinate createCoordinate(int i, int j) {
    Coordinate result;
    result.i = i;
    result.j = j;
    return result;
}

void copyBoard(Board targetBoard, Board copyFromBoard) {
    int i, j;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            targetBoard[i][j] = copyFromBoard[i][j];
        }
    }

}

void printUserBoard(Board board) {
    Game game;
    game.user_matrix = board;
    game.fixed_matrix = (BoolBoard) board;
    game.error_matrix = (BoolBoard) board;
    printBoard(&game);
}  /*TODO: delete*/

int getBoardValue(Board board, Coordinate coordinate) {
    return board[coordinate.i][coordinate.j];
}

void setBoardValue(Board board, Coordinate coordinate, int value) {
    board[coordinate.i][coordinate.j] = value;
}

Bool getBoolBoardValue(BoolBoard board, Coordinate coordinate) {
    return board[coordinate.i][coordinate.j];
}

void setBoolBoardValue(BoolBoard board, Coordinate coordinate, Bool value) {
    board[coordinate.i][coordinate.j] = value;
}
