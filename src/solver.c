#include <stdio.h>
#include "solver.h"

/* Solver module - responsible for all the board solving logic -
 * contains the exhaustive backtracking logic and all the board-solving aiding functions */

/* Cell Functions*/

int getEmptyCells(Board board, Coordinate *emptyCells) {
    int i, j, emptyCount = 0;

    /* go over each cell of the matrix*/
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            if (board[i][j] == 0) {
                /* if empty*/
                emptyCells[emptyCount] = createCoordinate(i, j);
                emptyCount++;
            }
        }
    }
    return emptyCount;
}

int getFilledCells(Board board, Coordinate *emptyCells) {
    int i, j, filledCount = 0;

    /* go over each cell of the matrix*/
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            if (board[i][j] != 0) {
                /* if filled*/
                emptyCells[filledCount] = createCoordinate(i, j);
                filledCount++;
            }
        }
    }
    return filledCount;
}

/**
 * fills neighbours array with the coordinate neighbours in the matrix (excluding itself)
 * the length of this array is constant
 * @param coordinate
 * @param neighbours
 */
void calculateCoordinateNeighbours(Coordinate coordinate, Coordinate *neighbours) {
    int i, j, k, neighboursCreated = 0;

    /* find leftmost coordinate*/
    Coordinate leftMostBlockCoordinate = createCoordinate(
            coordinate.i - (coordinate.i % g_gameDim.m),
            coordinate.j - (coordinate.j % g_gameDim.n)
    );

    /* go over all cell in the block expect of the one's in the row/column */
    for (i = leftMostBlockCoordinate.i + 0; i < leftMostBlockCoordinate.i + g_gameDim.m; ++i) {
        for (j = leftMostBlockCoordinate.j + 0; j < leftMostBlockCoordinate.j + g_gameDim.n; ++j) {
            if (i != coordinate.i && j != coordinate.j) {
                neighbours[neighboursCreated] = createCoordinate(i, j);
                neighboursCreated++;
            }
        }
    }

    /* go over all cells in the column and row*/
    for (k = 0; k < g_gameDim.N; ++k) {
        if (coordinate.i != k) {
            neighbours[neighboursCreated] = createCoordinate(k, coordinate.j);
            neighboursCreated++;
        }

        if (coordinate.j != k) {
            neighbours[neighboursCreated] = createCoordinate(coordinate.i, k);
            neighboursCreated++;
        }
    }
}

Bool hasEmptyCellWithNoPossibleValues(Board board) {
    Bool returnValue = false;
    Coordinate *emptyCells;
    int *possibleValues;
    int numOfEmpty;
    int numOfPossibleValues;
    int i;

    emptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    numOfEmpty = getEmptyCells(board, emptyCells);

    possibleValues = (int *) smartMalloc(g_gameDim.cellsCount * sizeof(int));

    for (i = 0; i < numOfEmpty; i++) {
        numOfPossibleValues = getPossibleValues(board, emptyCells[i], possibleValues);
        if (numOfPossibleValues == 0) {
            returnValue = true;
            break;
        }
    }

    free(emptyCells);
    free(possibleValues);

    return returnValue;
}

/* Values Functions*/

int getPossibleValues(Board board, Coordinate coordinate, int *possibleValues) {
    int i, possibleValuesCount = 0;
    Coordinate *neighbours;
    neighbours = (Coordinate *) smartMalloc(g_gameDim.cellNeighboursCount * sizeof(Coordinate));

    /* init an array of all numbers available*/
    for (i = 0; i < g_gameDim.N; ++i) {
        possibleValues[i] = i + 1;
    }

    calculateCoordinateNeighbours(coordinate, neighbours);

    /* zeroing values from the array of used numbers*/
    for (i = 0; i < g_gameDim.cellNeighboursCount; ++i) {
        int neighbourValue = board[neighbours[i].i][neighbours[i].j];
        if (neighbourValue != 0) {
            possibleValues[neighbourValue - 1] = 0;
        }
    }

    /*closing the gap of zeroes*/
    /*for each value in array*/
    for (i = 0; i < g_gameDim.N; ++i) {
        /* if it isn't zero*/
        if (possibleValues[i] != 0) {
            /*put the non zero value to the next actual filled cell*/
            possibleValues[possibleValuesCount] = possibleValues[i];
            possibleValuesCount++;
        }
    }

    /*zeroing the rest of the array - not necessary but cleaner*/
    for (i = possibleValuesCount; i < g_gameDim.N; ++i) {
        possibleValues[i] = 0;
    }

    free(neighbours);
    /*return the not zeroes values amount*/
    return possibleValuesCount;
}

Bool isPossibleValue(Board board, Coordinate coordinate, int value) {
    int *possibleValues;
    int possibleValuesCount;
    int i;
    Bool returnValue = false;

    possibleValues = (int *) smartMalloc(g_gameDim.N * sizeof(int));
    possibleValuesCount = getPossibleValues(board, coordinate, possibleValues);

    /* checks if one of the possible values is the value*/
    for (i = 0; i < possibleValuesCount; i++) {
        if (possibleValues[i] == value) {
            returnValue = true;
            break;
        }
    }

    free(possibleValues);
    return returnValue;
}

/* Board Functions*/

Bool isBoardFull(Board board) {
    /* implemented by checks hat no empty cells exists
     * *** does not mean the board is correct ***
     * */

    Coordinate *emptyCells;
    int emptyCellsCount;

    emptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    emptyCellsCount = getEmptyCells(board, emptyCells);
    free(emptyCells);

    /* if 0 empty cells */
    return emptyCellsCount == 0;
}

void clearBoard(Board board) {
    /* Zeroing all board cells */
    int i, j;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            board[i][j] = 0;
        }
    }
}

void clearBoolBoard(BoolBoard board) {
    /* put false in all board cells */
    int i, j;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            board[i][j] = false;
        }
    }
}

Bool isBoardErroneous(Board board) {
    /* implemented by checking neighbours values of each cell */
    /* notice that if it's in a part of the flow of the program, there is more efficient way to validate this */

    Bool returnValue = false;
    Coordinate *nonEmptyCells;
    Coordinate *coordinateNeighbours;
    int nonEmptyCount;
    int i, j;

    nonEmptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    nonEmptyCount = getFilledCells(board, nonEmptyCells);

    coordinateNeighbours = (Coordinate *) smartMalloc(g_gameDim.cellNeighboursCount * sizeof(Coordinate));

    for (i = 0; i < nonEmptyCount; i++) {
        calculateCoordinateNeighbours(nonEmptyCells[i], coordinateNeighbours);

        /* checks that it's value does not exist on any of the neighbour cells */
        for (j = 0; j < g_gameDim.cellNeighboursCount; j++) {
            if (getBoardValue(board, nonEmptyCells[i]) == getBoardValue(board, coordinateNeighbours[j])) {
                returnValue = true;
                break;
            }
        }
        if (returnValue == true) {
            break;
        }
    }

    free(nonEmptyCells);
    free(coordinateNeighbours);

    return returnValue;
}

Bool isBoardComplete(Board board) {
    /* checks if the board is full and valid */
    return (isBoardFull(board) == true) && (isBoardErroneous(board) == false);
}

/* Error Matrix Functions*/

/**
 * this function updates the relevant neighbors error matrix:
   1) we assume we're being called with set to a zero value
   2) we assume the user board cell is already being updated to 0
   3) we get the neighbors list
   3) for each neighbor we clear the error matrix cell
   4) then, for every non-zero neighbor we simulate a set in order to see if he's erroneous
   4) after updating all of the neighbors the error matrix is correctly updated after a zero set
 * @param game
 * @param input
 */
void updateAfterClearErrorMatrix(Game *game, Input input) {
    int k;
    Input in;
    Coordinate *neighbours;
    neighbours = (Coordinate *) smartMalloc(g_gameDim.cellNeighboursCount * sizeof(Coordinate));
    calculateCoordinateNeighbours(input.coordinate, neighbours);

    for (k = 0; k < g_gameDim.cellNeighboursCount; k++) {
        game->error_matrix[neighbours[k].i][neighbours[k].j] = 0;
    }

    for (k = 0; k < g_gameDim.cellNeighboursCount; k++) {
        in.coordinate = createCoordinate(neighbours[k].i, neighbours[k].j);
        in.value = game->user_matrix[in.coordinate.i][in.coordinate.j];
        if (in.value != 0) {
            updateAfterSetErrorMatrix(game, in);
        }
    }

    free(neighbours);
    game->error_matrix[input.coordinate.i][input.coordinate.j] = 0;
}

/**
 * this function updates the relevant neighbors error matrix:
 1) we assume we're being called with a different then zero value
 2) we get the neighbors list
 3) for each neighbor we check if it has the same value of the set cell
    if it does then we update the neighbor's error matrix cell to 1
 4) if we had to update the error matrix for the neighbor that means the
    cell set itself is erroneous, and we mark it as such
 * @param game
 * @param input
 */
void updateAfterSetErrorMatrix(Game *game, Input input) {
    int k;
    Bool flag = false;
    Coordinate *neighbours;
    neighbours = (Coordinate *) smartMalloc(g_gameDim.cellNeighboursCount * sizeof(Coordinate));
    calculateCoordinateNeighbours(input.coordinate, neighbours);
    for (k = 0; k < g_gameDim.cellNeighboursCount; k++) {
        if (game->user_matrix[neighbours[k].i][neighbours[k].j] == input.value) {
            game->error_matrix[neighbours[k].i][neighbours[k].j] = 1;
            flag = true;
        }
    }
    if (flag) {
        game->error_matrix[input.coordinate.i][input.coordinate.j] = 1;
    }
    free(neighbours);
}

/**
 * this function updates the whole error matrix:
   1) first it clears the error matrix
   2) then it goes every non-empty cell and simulates a set to that cell
   3) the simulation of the set to the current board translated
      to the "updateAfterSetErrorMatrix" function which updates all of
      the erroneous neighbor cells
   4) after performing it to all cells, the error matrix correctly updated
 * @param game
 */
void updateWholeErrorMatrix(Game *game) {
    int i, j, val;
    Input input;
    Coordinate cor;

    clearBoolBoard(game->error_matrix);
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            cor.i = i;
            cor.j = j;
            val = game->user_matrix[i][j];
            input.coordinate = cor;
            input.value = val;

            if (val != 0) {
                updateAfterSetErrorMatrix(game, input);
            }
        }
    }
}

/* Count solutions */

/* Stack Node */
typedef struct _StackFrame {
    /* Stack Data (local frame variables) */
    int start;
    Coordinate currentCoordinate;
    int *possibleValues;
    int possibleValuesCount;
    int nextValue;
    Bool beforeChildPoped;
    Bool isInitialized;

    /* Stack Pointer */
    struct _StackFrame *parentScope;
} StackFrame;

/**
 * This function will exhaust all possible solutions to the board, using backtracking
   that stimulates recursion with stack of frames
 * @param board
 * @return Num of possible solutions
 */
int getNumOfPossibleSolutions(Board board) {
    Coordinate *emptyCells;
    int emptyCellsCount;
    int returnValue = 0;
    Board tempBoard;
    StackFrame *StackTop; /* stack of frames */


    /* make a copy of the current board to solve on*/
    tempBoard = createBoard();
    copyBoard(tempBoard, board);


    /* needed for the init of the recursion */
    emptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    emptyCellsCount = getEmptyCells(tempBoard, emptyCells);

    /* init stack */
    StackTop = (StackFrame *) smartMalloc(sizeof(StackFrame));

    /* push first frame to stack */
    StackTop->parentScope = NULL;
    StackTop->beforeChildPoped = true;
    StackTop->isInitialized = false;
    StackTop->start = 0;

    /* Begin Recursion part - separated to 4 stages that has to happen in the following order,
     * init frame (1) (happens when the frame is pushed to the stack),
     * before push child frame (2),
     * handling child result (3)
     * destroying frame (4) (happens before popping the frame)
     *
     * stages 2,3 happens repeatably until all the options exhausted
     */
    do {
        if (StackTop->isInitialized == false) {
            /* stage (1) - initialize */
            StackTop->currentCoordinate = emptyCells[StackTop->start];
            StackTop->possibleValues = (int *) smartMalloc(g_gameDim.N * sizeof(int));
            StackTop->possibleValuesCount = getPossibleValues(
                    tempBoard, StackTop->currentCoordinate, StackTop->possibleValues
            );
            StackTop->isInitialized = true;
        }

        /* as long there are more possible values that we didn't check*/
        while (StackTop->possibleValuesCount > 0) {
            if (StackTop->beforeChildPoped == true) {
                /* stage (2)*/

                /* remove the lowest option*/
                StackTop->nextValue = removeArrayIndex(StackTop->possibleValues, StackTop->possibleValuesCount, 0);

                /*change the value to the next value*/
                setBoardValue(tempBoard, StackTop->currentCoordinate, StackTop->nextValue);

                /* if this is the last cell of the board*/
                if (StackTop->start == emptyCellsCount - 1) {

                    /* found a possible solution */
                    returnValue += 1;

                    /* backtrack from the last value*/
                    setBoardValue(tempBoard, StackTop->currentCoordinate, 0);

                    /* if got to here, the value must be the only possible value, then no need to go over to next child */
                    break;
                } else {
                    /*push*/

                    /* init child frame */
                    StackFrame *newScope = (StackFrame *) smartMalloc(sizeof(StackFrame));
                    newScope->parentScope = StackTop;
                    newScope->beforeChildPoped = true;
                    newScope->isInitialized = false;
                    newScope->start = StackTop->start + 1;

                    /* when the parent frame will be in the top again, it will go to stage 3 */
                    StackTop->beforeChildPoped = false;
                    StackTop = newScope;

                    /* because the child frame is not initialized yet */
                    break;
                }
            } else {
                /* stage (3) */

                /* clears the exhausted value option (backtrack) */
                setBoardValue(tempBoard, StackTop->currentCoordinate, 0);

                /* decrease the available options amount left */
                StackTop->possibleValuesCount--;

                /* marking ready for next child to be pushed */
                StackTop->beforeChildPoped = true;

                /* for clarity - to exhaust the next possible value if exist*/
                continue;
            }
        }

        /* child frames are not initialized, thus will not get to stage (4), and go to stage (1) instead */
        if (StackTop->isInitialized == true) {
            /* stage (4)*/
            StackFrame *parentScope = StackTop->parentScope;

            free(StackTop->possibleValues);
            free(StackTop);

            /* pop */
            StackTop = parentScope;
        }

    } while (StackTop != NULL); /* while the stack isn't empty */

    /* End */

    free(emptyCells);
    destroyBoard(tempBoard, g_gameDim);

    return returnValue;
}
