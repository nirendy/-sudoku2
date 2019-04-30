#include "game_aux.h"


/*Game Auxiliary module - responsible for aiding the game module with specific game-related functions*/

/* Legal Checks related */
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


/* Perform Commands Related*/
/*board-changing commands related*/

void actIfBoardFullAndValid(Game *game) {
    if (g_mode == Solve && isBoardFull(game->user_matrix)) {
        if (!isGameErroneous(game)) {
            printPrompt(PSuccess, 0);
            g_mode = Init;
        } else {
            printPrompt(PWrongSolution, 0);
        }
    }
}

/* Edit related*/
void clearGame(Game *game) {
    clearBoard(game->user_matrix);
    clearBoolBoard(game->fixed_matrix);
    clearBoolBoard(game->error_matrix);
}

/* Set related*/
void setCoordinate(Game *game, Input input) {
    /*beyond setting the value to the board, it call the functions responsible for the error matrix
     * each set is considered as set to zero and then to it's real value (if exists)
     * in that way we can clear and then fill the error matrix correctly
     * using the error matrix functions implemented (and further documented) in the solver module*/


    game->user_matrix[input.coordinate.i][input.coordinate.j] = 0;
    updateAfterClearErrorMatrix(game, input);
    if (input.value != 0) {
        updateAfterSetErrorMatrix(game, input);
        game->user_matrix[input.coordinate.i][input.coordinate.j] = input.value;
    }
}

/* Undo, Redo related */
void printChange(int i, int j, int value) {
    printf("The value of the cell <%d,%d> has been set to %d\n", j + 1, i + 1, value);
}

/*   Link list History related   */

/*      local functions       */
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

void performSetsFromRedoList(Game *game, Input *sets, int len) {
    int k;
    for (k = 0; k < len; k++) {
        setCoordinate(game, sets[k]);
    }
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

/**
 *
 * @param original (board)
 * @param final (board)
 * @return Number of diffs between boards
 */
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

/*      public functions       */

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

int updateHistoryList(Game *game, Board final) {

    int numOfSets;
    Input *redoInputs;
    Input *undoInputs;

    Board original = game->user_matrix;
    numOfSets = numOfDiffs(original, final);
    if (numOfSets == 0) {
        destroyBoard(final, g_gameDim);
        return 0;
    }
    redoInputs = (Input *) smartMalloc(numOfSets * sizeof(Input));
    undoInputs = (Input *) smartMalloc(numOfSets * sizeof(Input));

    diffToRedoUndoLists(original, final, redoInputs, undoInputs);
    insertInputsToList(redoInputs, undoInputs, numOfSets);
    performSetsFromRedoList(game, redoInputs, numOfSets); /*update the user board*/

    destroyBoard(final, g_gameDim);
    free(redoInputs);
    free(undoInputs);
    return  numOfSets;
}

/* Validate related */
Bool isSolvableBoard(Board board) {
    Bool isValid;
    Board solutionBoard = createBoard();
    isValid = fillSolutionMatrix(board, solutionBoard);
    destroyBoard(solutionBoard, g_gameDim);
    return isValid;
}

/* Generate related */
int chooseCellsToFill(Board board, Coordinate *cellsToFill, int sizeToFill) {

    Coordinate *emptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    int numOfEmpty = getEmptyCells(board, emptyCells);
    chooseRandCords(emptyCells, numOfEmpty, cellsToFill, sizeToFill);
    free(emptyCells);
    return numOfEmpty;
}

/**
 *
 * @param board
 * @param cellsToFill list of cells to fill X cells from
 * @param numToFill (X)
 * @return false if couldn't fill all X cells legally legally
 */
Bool fillXRandomCells(Board board, Coordinate *cellsToFill, int numToFill) {
    int k, numOfPossibleValues;
    int *possibleValues = (int *) smartMalloc(g_gameDim.N * sizeof(int *));

    for (k = 0; k < numToFill; k++) {
        numOfPossibleValues = getPossibleValues(board, cellsToFill[k], possibleValues);
        if (numOfPossibleValues == 0) {
            free(possibleValues);
            return false;
        }
        board[cellsToFill[k].i][cellsToFill[k].j] = possibleValues[randLimit(numOfPossibleValues)];
    }

    free(possibleValues);
    return true;
}

/**
 * fill the cellsToClear array with coordinates to clear
 * @param board
 * @param cellsToClear
 * @param numToClear
 */
void chooseCellsToClear(Board board, Coordinate *cellsToClear, int numToClear) {

    /* the board is fully solved, therefore: #ofFilledCells = cellCount*/
    Coordinate *filledCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    getFilledCells(board, filledCells);
    chooseRandCords(filledCells, g_gameDim.cellsCount, cellsToClear, numToClear);

    free(filledCells);
}

/**
 *
 * @param board
 * @param cellsToClear
 * @param numToClear
 */
void clearRandomCells(Board board, Coordinate *cellsToClear, int numToClear) {

    int k;
    for (k = 0; k < numToClear; k++) {
        board[cellsToClear[k].i][cellsToClear[k].j] = 0;
    }

}

/* Autofill related */
/**
 * fills cells with only one possible value
 * @param game
 * @param board
 */
void fillObviousValues(Game *game, Board board) {

    Coordinate *emptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    int *possibleValues = (int *) smartMalloc(g_gameDim.cellsCount * sizeof(int));

    int numOfEmpty = getEmptyCells(board, emptyCells);
    int numOfPossibleValues;

    int k;
    for (k = 0; k < numOfEmpty; k++) {
        numOfPossibleValues = getPossibleValues(game->user_matrix, emptyCells[k], possibleValues);
        if (numOfPossibleValues == 1) {
            board[emptyCells[k].i][emptyCells[k].j] = possibleValues[0];
        }
    }

    free(emptyCells);
    free(possibleValues);
}


