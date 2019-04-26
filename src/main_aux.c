#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main_aux.h"
#include "solver.h"
#include "parser.h"
#include "file_handler.h"
#include "linked_list.h"

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

void printChange(int i, int j, int value) {
    printf("The value of the cell <%d,%d> set back to %d\n", i + 1, j + 1, value);
}

Coordinate createCoordinate(int i, int j) {
    Coordinate result;
    result.i = i;
    result.j = j;
    return result;
}

int randLimit(int limit) {
    return rand() % limit;
}

void setGameDim(int n, int m) {
    g_gameDim.n = n;
    g_gameDim.m = m;
    g_gameDim.N = n * m;
    g_gameDim.cellsCount = g_gameDim.N * g_gameDim.N;
    g_gameDim.cellNeighboursCount = 3 * g_gameDim.N - n - m - 1;
}

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

Bool isGameErroneous(Game *game) {
    int i, j;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            if (game->error_matrix[i][j] == 1) {
                return true;
            }
        }
    }
    return false;
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

void copyBoard(Board targetBoard, Board copyFromBoard) {
    int i, j;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            targetBoard[i][j] = copyFromBoard[i][j];
        }
    }

}

Bool isCommandAllowedInMode(Mode mode, Command command) {
    switch (command) {
        case COMMAND_SOLVE: {
            return true;
        }
        case COMMAND_EDIT: {
            return true;
        }
        case COMMAND_MARK_ERRORS: {
            return (mode == Solve);
        }
        case COMMAND_PRINT_BOARD: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_SET: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_VALIDATE: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_GUESS: {
            return (mode == Solve);
        }
        case COMMAND_GENERATE: {
            return (mode == Edit);
        }
        case COMMAND_UNDO: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_REDO: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_SAVE: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_HINT: {
            return (mode == Solve);
        }
        case COMMAND_GUESS_HINT: {
            return (mode == Solve);
        }
        case COMMAND_NUM_SOLUTIONS: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_AUTOFILL: {
            return (mode == Solve);
        }
        case COMMAND_RESET: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_EXIT: {
            return true;
        }
        case COMMAND_INVALID:
        default: {
            printf("Unreachable Code Error");
            return false;
        }
    }
}

void terminate(Game *game, FinishCode finishCode) {
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

Bool askUserForNextTurn(Input *input) {

    FinishCode finishCode;
    printPrompt(PNextCommand, 0);
    finishCode = parseCommand(input);
    if (!(finishCode == FC_SUCCESS || finishCode == FC_INVALID_RECOVERABLE)) {
        terminate(NULL, finishCode);
        return false;
    }

    if (finishCode == FC_INVALID_RECOVERABLE) { return false; }

    return true;
}

void setMode(Mode *mode, Mode newMode) {
    *mode = newMode;
}

void performUndo(Game *game, DataNode *currDataNode, Bool toPrint) {
    Input input;
    currDataNode = getLastDataNode(currDataNode);
    while (currDataNode->isFirst == false) {
        input = currDataNode->undoInput;
        if (toPrint) { printChange(input.coordinate.i, input.coordinate.j, input.value); }
        setCoordinate(game, input);
        currDataNode = currDataNode->prev;
    }
}

void performRedo(Game *game, DataNode *currDataNode) {
    Input input;
    currDataNode = getFirstDataNode(currDataNode);
    currDataNode = currDataNode->next;
    while (currDataNode != NULL) {
        input = currDataNode->redoInput;
        printChange(input.coordinate.i, input.coordinate.j, input.value);
        setCoordinate(game, input);
        currDataNode = currDataNode->next;
    }
}

void updateRedoUndoInputsAfterSingleSet(Game *game, Input in, Input *redo, Input *undo) {

    redo->coordinate = in.coordinate;
    undo->coordinate = in.coordinate;

    redo->value = in.value;
    undo->value = game->user_matrix[in.coordinate.i][in.coordinate.j]; /*only good for a single set*/

}

void insertInputsToList(Input *redoInputs, Input *undoInputs, int numOfInputs) {
    int k;

    g_curNode = insertAfterNode(g_curNode);
    for (k = 0; k < numOfInputs; k++) {
        g_curNode->currDataNode = insertAfterDataNode(g_curNode->currDataNode, redoInputs[k], undoInputs[k]);
    }

}

void chooseRandCords(Coordinate *source, int sourceSize, Coordinate *target, int targetSize) {
    int i, r, limit;
    int *randIndexes = (int *) smartMalloc(sourceSize * sizeof(int));

    for (i = 0; i < sourceSize; i++) { randIndexes[i] = i; }
    limit = sourceSize;
    for (i = 0; i < targetSize; i++) {
        r = randLimit(limit);
        target[i] = source[randIndexes[r]];
        randIndexes[r] = randIndexes[limit - 1];
        limit--;
    }

    free(randIndexes);
}

int diffToRedoUndoLists(Board original, Board final, Input *redoList, Input *undoList) {

    int i, j, listIndex = 0;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            if (original[i][j] != final[i][j]) {
                redoList[listIndex].coordinate.i = i;
                redoList[listIndex].coordinate.j = j;
                undoList[listIndex].coordinate.i = i;
                undoList[listIndex].coordinate.j = j;
                redoList[listIndex].value = final[i][j];
                undoList[listIndex].value = original[i][j];
                listIndex++;
            }
        }
    }

    return listIndex;
}

int numOfDiffs(Board original, Board final) {
    int i, j, diffs = 0;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            if (original[i][j] != final[i][j]) {
                diffs++;
            }
        }
    }
    return diffs;
}

void performSetsFromRedoList(Game *game, Input *sets, int len) {
    int k;
    for (k = 0; k < len; k++) {
        setCoordinate(game, sets[k]);
    }
}

void updateHistoryList(Game *game, Board final) {

    int numOfSets;
    Input *redoInputs;
    Input *undoInputs;

    Board original = game->user_matrix;
    numOfSets = numOfDiffs(original, final);
    if (numOfSets == 0) {
        destroyBoard(final, g_gameDim);
        return;
    }
    redoInputs = (Input *) smartMalloc(numOfSets * sizeof(Input));
    undoInputs = (Input *) smartMalloc(numOfSets * sizeof(Input));

    diffToRedoUndoLists(original, final, redoInputs, undoInputs);
    insertInputsToList(redoInputs, undoInputs, numOfSets);
    performSetsFromRedoList(game, redoInputs, numOfSets); /*update the user board*/

    destroyBoard(final, g_gameDim);
    free(redoInputs);
    free(undoInputs);
}

Bool fillXRandomCells(Board board, Coordinate *cellsToFill, int numToFill) {

    int k, numOfPossibleValues;
    int *possibleValues = (int *) smartMalloc(g_gameDim.N * sizeof(int *));

    for (k = 0; k < numToFill; k++) {
        numOfPossibleValues = getPossibleValues(board, cellsToFill[k], possibleValues);
        if (numOfPossibleValues == 0) { return false; }
        board[cellsToFill[k].i][cellsToFill[k].j] = possibleValues[randLimit(numOfPossibleValues)];
    }
    return true;
}

void clearRandomCells(Board board, Coordinate *cellsToClear, int numToClear) {

    int k;
    for (k = 0; k < numToClear; k++) {
        board[cellsToClear[k].i][cellsToClear[k].j] = 0;
    }

}

int chooseCellsToFill(Board board, Coordinate *cellsToFill, int sizeToFill) {

    Coordinate *emptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    int numOfEmpty = getEmptyCells(board, emptyCells);
    chooseRandCords(emptyCells, numOfEmpty, cellsToFill, sizeToFill);
    free(emptyCells);
    return numOfEmpty;
}

void chooseCellsToClear(Board board, Coordinate *cellsToClear, int numToClear) {

    Coordinate *filledCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    getFilledCells(board, filledCells); /* TODO: before submitting, use the returned value*/
    chooseRandCords(filledCells, g_gameDim.cellsCount, cellsToClear, numToClear);

    free(filledCells);
}

void fillObviousValues(Board board) {

    Coordinate *emptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    int *possibleValues = (int *) smartMalloc(g_gameDim.cellsCount * sizeof(int));

    int numOfEmpty = getEmptyCells(board, emptyCells);
    int numOfPossibleValues;
    int numOfCellsToFill = 0;

    int k;
    for (k = 0; k < numOfEmpty; k++) {
        numOfPossibleValues = getPossibleValues(board, emptyCells[k], possibleValues);
        if (numOfPossibleValues == 1) {
            board[emptyCells[k].i][emptyCells[k].j] = possibleValues[0];
            numOfCellsToFill++;
        }
    }

    free(emptyCells);
    free(possibleValues);
}

Bool performGuess(Game *game, Input input) {

    Board solutionBoard = createBoard();
    if (!guessBoard(game->user_matrix, solutionBoard, input.threshold)) {
        destroyBoard(solutionBoard, g_gameDim);
        return false;
    }

    clearListFromNode(g_curNode->next);
    g_curNode->next = NULL;
    updateHistoryList(game, solutionBoard);  /*destroys newBoard*/
    return true;
}

Bool performGenerate(Game *game, Input input) {

    /*variables deceleration*/
    Bool success;
    const int NUM_OF_ITERATIONS = 1000;
    int k, numToFill, numToClear;

    Board newBoard, solutionBoard;
    Coordinate *cellsToFill, *cellsToClear;

    /*variables deceleration*/
    numToFill = input.gen1;
    numToClear = g_gameDim.cellsCount - input.gen2;

    /*step 1 - fill the board */

    /*choose cells to fill*/
    cellsToFill = (Coordinate *) smartMalloc(numToFill * sizeof(Coordinate));

    /*try 1000 times to fill and solve*/
    success = false;
    newBoard = createBoard();
    solutionBoard = createBoard();

    for (k = 0; k < NUM_OF_ITERATIONS; k++) {
        chooseCellsToFill(game->user_matrix, cellsToFill, numToFill);
        copyBoard(newBoard, game->user_matrix);
        if (!fillXRandomCells(newBoard, cellsToFill, numToFill)) { continue; }
        if (!fillSolutionMatrix(newBoard, solutionBoard)) { continue; }

        /*successful solution*/
        copyBoard(newBoard, solutionBoard);
        success = true;
        break;
    }
    destroyBoard(solutionBoard, g_gameDim);
    free(cellsToFill);


    if (!success) {
        printError(EGenerationFailed);
        destroyBoard(newBoard, g_gameDim);
        return success;
    }

    /*step 2 - clear cells from the board */
    if (numToClear > 0) {
        cellsToClear = (Coordinate *) smartMalloc(numToClear * sizeof(Coordinate));
        chooseCellsToClear(newBoard, cellsToClear, numToClear);
        clearRandomCells(newBoard, cellsToClear, numToClear);
        free(cellsToClear);
    }

    /*step 3 - perform changes and update the redo/undo list */
    clearListFromNode(g_curNode->next);
    g_curNode->next = NULL;
    updateHistoryList(game, newBoard); /*destroys newBoard*/
    return true;

}

void performAutoFill(Game *game) {

    Board newBoard = createBoard();
    copyBoard(newBoard, game->user_matrix);
    fillObviousValues(newBoard);
    updateHistoryList(game, newBoard); /*destroys newBoard*/
}

Bool checkLegalInput(Input input, Game *game) {
    /*generate_command vars*/
    Coordinate *tempCorArray;
    int numOfEmptyCells;

    switch (input.command) {
        case COMMAND_SOLVE: {
            return true;
        }
        case COMMAND_EDIT: {
            return true;
        }
        case COMMAND_MARK_ERRORS: {
            /*     parameter range check    */
            if (!(input.value == 0 || input.value == 1)) {
                printError(EInvalidFirstParam);
                printf("parameter must be a binary number - 0 or -1\n");
                return false;
            }


            return true;
        }
        case COMMAND_PRINT_BOARD: {
            return true;
        }

        case COMMAND_SET: {
            /*     parameter range check    */

            /*First Parameter Check*/
            if (!(input.coordinate.i >= 0 && input.coordinate.i <= g_gameDim.N - 1)) {
                printError(EInvalidFirstParam);
                printf("parameter must be an integer number between 1 and %d\n", g_gameDim.N);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.coordinate.j >= 0 && input.coordinate.j <= g_gameDim.N - 1)) {
                printError(EInvalidSecondParam);
                printf("parameter must be an integer number between 1 and %d\n", g_gameDim.N);
                return false;
            }

            /*Third Parameter Check*/
            if (!(input.value >= 0 && input.value <= g_gameDim.N)) {
                printError(EInvalidThirdParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.N);
                return false;
            }

            /*   is parameter legal in current board state check    */

            if (isCoordinateFixed(game, input.coordinate)) {
                printError(ECellIsFixed);
                return false;
            }


            return true;
        }
        case COMMAND_VALIDATE: {
            /*   is parameter legal in current board state check    */

            if (isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }
            return true;
        }
        case COMMAND_GUESS: {
            /*     parameter range check    */

            if (!(input.threshold >= 0 && input.threshold <= 1)) {
                printError(EInvalidFirstParam);
                printf("parameter must be a float number between 0 and 1\n");
                return false;
            }

            /*   is parameter legal in current board state check    */

            if (isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }
            return true;
        }
        case COMMAND_GENERATE: {
            /*     parameter range check    */

            /*First Parameter Check*/
            if (!(input.gen1 >= 0 && input.gen1 <= g_gameDim.cellsCount)) {
                printError(EInvalidFirstParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.cellsCount);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.gen2 >= 0 && input.gen2 <= g_gameDim.cellsCount)) {
                printError(EInvalidSecondParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.cellsCount);
                return false;
            }

            /*   is parameter legal in current board state check    */


            tempCorArray = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
            numOfEmptyCells = getEmptyCells(game->user_matrix, tempCorArray);
            free(tempCorArray);

            if (input.gen1 > numOfEmptyCells) {
                printError(EInvalidFirstParam);
                printf("parameter must be smaller or equal than the number of empty cells: %d\n",
                       numOfEmptyCells);
                return false;
            }

            return true;
        }
        case COMMAND_UNDO: {
            /*   is parameter legal in current board state check    */

            if (g_curNode->isFirst) {
                printError(EUndoUnavailable);
                return false;
            }
            return true;
        }

        case COMMAND_REDO: {
            /*   is parameter legal in current board state check    */

            if (g_curNode->next == NULL) {
                printError(ERedoUnavailable);
                return false;
            }
            return true;
        }
        case COMMAND_SAVE: {
            /*   is parameter legal in current board state check    */

            if (g_mode == Edit && isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }


            return true;
        }
        case COMMAND_HINT: {
            /*     parameter range check    */

            /*First Parameter Check*/
            if (!(input.coordinate.i >= 0 && input.coordinate.i <= g_gameDim.N - 1)) {
                printError(EInvalidFirstParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.N - 1);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.coordinate.j >= 0 && input.coordinate.j <= g_gameDim.N - 1)) {
                printError(EInvalidSecondParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.N - 1);
                return false;
            }

            /*   is parameter legal in current board state check    */
            if (isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }

            if (isCoordinateFixed(game, input.coordinate)) {
                printError(ECellIsFixed);
                return false;
            }

            if (!isCoordinateEmpty(game, input.coordinate)) {
                printError(ECellIsNotEmpty);
                return false;
            }

            return true;
        }
        case COMMAND_GUESS_HINT: {
            /*     parameter range check    */

            /*First Parameter Check*/
            if (!(input.coordinate.i >= 0 && input.coordinate.i <= g_gameDim.N - 1)) {
                printError(EInvalidFirstParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.N - 1);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.coordinate.j >= 0 && input.coordinate.j <= g_gameDim.N - 1)) {
                printError(EInvalidSecondParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.N - 1);
                return false;
            }

            /*   is parameter legal in current board state check    */
            if (isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }

            if (isCoordinateFixed(game, input.coordinate)) {
                printError(ECellIsFixed);
                return false;
            }

            if (!isCoordinateEmpty(game, input.coordinate)) {
                printError(ECellIsNotEmpty);
                return false;
            }

            return true;
        }
        case COMMAND_NUM_SOLUTIONS: {
            /*   is parameter legal in current board state check    */

            if (isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }
            return true;
        }
        case COMMAND_AUTOFILL: {
            /*   is parameter legal in current board state check    */

            if (isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }
            return true;
        }
        case COMMAND_RESET: {
            return true;
        }
        case COMMAND_EXIT: {
            return true;
        }
        case COMMAND_INVALID: {
            printf("Unreachable Code Error\n");
            terminate(game, FC_UNEXPECTED_ERROR);
            return false;
        }


    }
    return false;
}

void executeCommand(Input input, Game **gameP) {

    Game *game = *gameP;
    Mode *modePtr = &g_mode;
    Bool success = false;
    Board solutionBoard;

    /*this commands cannot fail in this stage so its safe to clear the redo list
     * guess and generate command can fail so clearing the redo list is done after we get success*/

    if (input.command == COMMAND_SET ||
        input.command == COMMAND_AUTOFILL) {
        clearListFromNode(g_curNode->next);
        g_curNode->next = NULL;
    }

    switch (input.command) {
        case COMMAND_SOLVE: {
            Game *newGame = createGameFromFile(input.path);
            if (newGame != NULL) {
                if (game != NULL) {          /*skip if first game*/
                    destroyGame(game);
                }
                *gameP = newGame;
                game = newGame;
                setMode(modePtr, Solve);
                updateWholeErrorMatrix(game);
                g_curNode = getFirstNode(g_curNode);
                clearListFromNode(g_curNode->next);
                g_curNode->next = NULL;
                success = true;
            }

            break;
        }
        case COMMAND_EDIT: {
            Game *newGame;
            if (strlen(input.path) == 0) {
                setGameDim(3, 3);
                newGame = createGame();
                clearGame(newGame);

            } else {
                newGame = createGameFromFile(input.path);
            }

            if (newGame != NULL) {
                if (game != NULL) {         /*skip if first game*/
                    destroyGame(game);
                }
                *gameP = newGame;
                game = newGame;
                setMode(modePtr, Edit);
                clearBoolBoard(game->fixed_matrix);
                updateWholeErrorMatrix(game);
                g_curNode = getFirstNode(g_curNode);
                clearListFromNode(g_curNode->next);
                g_curNode->next = NULL;
                success = true;
            }
            break;
        }
        case COMMAND_MARK_ERRORS: {
            if (input.value == 0) { g_markError = false; }
            else if (input.value == 1) { g_markError = true; }
            else { printf("Unreachable Code"); }

            break;
        }
        case COMMAND_PRINT_BOARD: {
            printBoard(game);
            break;
        }
        case COMMAND_SET: {
            Input redoInput, undoInput;
            updateRedoUndoInputsAfterSingleSet(game, input, &redoInput, &undoInput);
            insertInputsToList(&redoInput, &undoInput, 1);
            setCoordinate(game, input);
            if (g_mode == Solve && isFullUserBoard(game)) {
                if (!isGameErroneous(game)) {
                    printPrompt(PSuccess, 0);
                    g_mode = Init;
                } else {
                    printPrompt(PWrongSolution, 0);
                }
            }
            success = true; /*fail condition checked in isLegalMove*/
            break;
        }
        case COMMAND_VALIDATE: {
            validate(game);
            break;
        }
        case COMMAND_GUESS: {
            success = performGuess(game, input);
            break;
        }
        case COMMAND_GENERATE: {
            success = performGenerate(game, input);
            break;
        }
        case COMMAND_UNDO: {
            printPrompt(PPerformedChanges, 0);
            performUndo(game, g_curNode->currDataNode, true);
            g_curNode = g_curNode->prev;
            success = true; /*fail condition checked in isLegalMove*/
            break;
        }
        case COMMAND_REDO: {
            printPrompt(PPerformedChanges, 0);
            g_curNode = g_curNode->next;
            performRedo(game, g_curNode->currDataNode);
            success = true; /*fail condition checked in isLegalMove*/
            break;
        }
        case COMMAND_SAVE: {
            solutionBoard = createBoard();
            if (g_mode == Edit && !fillSolutionMatrix(game->user_matrix, solutionBoard)) {
                printError(EFUnsolvableBoard);
            } else { saveGameToFile(input.path, game); }
            destroyBoard(solutionBoard, g_gameDim);
            break;
        }
        case COMMAND_HINT: {
            hint(game, input);
            break;
        }
        case COMMAND_GUESS_HINT: {
            guessHint(game->user_matrix, input.coordinate);
            break;
        }
        case COMMAND_NUM_SOLUTIONS: {
            countPossibleSolutions(game->user_matrix);
            break;
        }
        case COMMAND_AUTOFILL: {
            performAutoFill(game);
            success = true;    /*fail condition checked in isLegalMove*/
            break;
        }
        case COMMAND_RESET: {
            while (!g_curNode->isFirst) {
                performUndo(game, g_curNode->currDataNode, false);
                g_curNode = g_curNode->prev;
            }
            success = true; /*fail condition checked in isLegalMove*/
            break;
        }
        case COMMAND_EXIT: { /*TODO: nir make sure that free all resorces*/
            terminate(game, FC_SUCCESS);
            break;
        }
        case COMMAND_INVALID: {
            printf("Unreachable Code Error\n");
            terminate(game, FC_UNEXPECTED_ERROR);
        }
    }

    if (success == true &&
        (input.command == COMMAND_SOLVE ||
         input.command == COMMAND_EDIT ||
         input.command == COMMAND_SET ||
         input.command == COMMAND_AUTOFILL ||
         input.command == COMMAND_UNDO ||
         input.command == COMMAND_REDO ||
         input.command == COMMAND_GENERATE ||
         input.command == COMMAND_GUESS ||
         input.command == COMMAND_RESET)) {
        printBoard(game);
    }

}

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

