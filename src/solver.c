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

Bool isFullUserBoard(Game *game) {
    Coordinate *emptyCells;
    int emptyCellsCount;

    emptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));

    emptyCellsCount = getEmptyCells(game->user_matrix, emptyCells);

    free(emptyCells);
    /* if 0 empty cells */
    return emptyCellsCount == 0;
}

void clearBoard(Board board) {
    int i, j;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            board[i][j] = 0;
        }
    }
}

void clearBoolBoard(BoolBoard board) {
    int i, j;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            board[i][j] = false;
        }
    }
}

Bool isBoardErroneous(Board board) {
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


/* Error Matrix Functions*/

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

void updateAfterSetErrorMatrix(Game *game, Input input) {
    int k;
    Bool flag = false;
    Coordinate *neighbours;
    neighbours = (Coordinate *) smartMalloc(g_gameDim.cellNeighboursCount * sizeof(Coordinate));
    calculateCoordinateNeighbours(input.coordinate, neighbours);
    for (k = 0; k < g_gameDim.cellNeighboursCount; k++) {
        if (game->user_matrix[neighbours[k].i][neighbours[k].j] == input.value) {
            flag = true;
            if (game->fixed_matrix[neighbours[k].i][neighbours[k].j] == 0) {
                game->error_matrix[neighbours[k].i][neighbours[k].j] = 1;
            }
        }
    }
    if (flag) {
        game->error_matrix[input.coordinate.i][input.coordinate.j] = 1;
    }
    free(neighbours);
}

void updateWholeErrorMatrix(Game *game) {
    int i, j, val;
    Input input;
    Coordinate cor;
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

FinishCode countPossibleSolutions(Board board) {
    /* This function will exhaust all possible solutions to the board, using backtracking
     * that stimulates recursion with stack of frames */

    Coordinate *emptyCells;
    int emptyCellsCount;
    int returnValue = 0;
    Board tempBoard;
    StackFrame *StackTop; /* frame stack */


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

    /* Begin Recursion part - separated to 4 parts that has to happen in the following order,
     * init frame (1) (happens when the frame is pushed to the stack),
     * before push child frame (2),
     * handling child result (3)
     * destroying frame (4) (happens before popping the frame)
     *
     * parts 2,3 happens repeatably until all the options exhausted
     */
    do {

        if (StackTop->isInitialized == false) {
            /* initialize (1)*/
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
                /* part (2)*/

                /* remove the lowest option*/
                StackTop->nextValue = removeArrayIndex(StackTop->possibleValues, StackTop->possibleValuesCount,
                                                       0);

                /*change the value to the next value*/
                tempBoard[StackTop->currentCoordinate.i][StackTop->currentCoordinate.j] = StackTop->nextValue;

                /* if this is the last cell
                 * */
                if (StackTop->start == emptyCellsCount - 1) {
                    /*pop*/
                    returnValue += 1;
                    tempBoard[StackTop->currentCoordinate.i][StackTop->currentCoordinate.j] = 0;

                    /*if got to here, the value must be the last than no need to go over to next child*/
                    break;
                } else {
                    /*push*/

                    StackFrame *newScope =
                            (StackFrame *) smartMalloc(sizeof(StackFrame));
                    newScope->parentScope = StackTop;
                    newScope->beforeChildPoped = true;
                    newScope->isInitialized = false;
                    newScope->start = StackTop->start + 1;

                    StackTop->beforeChildPoped = false;
                    StackTop = newScope;

                    /* so the new scope will be initialized*/
                    break;
                }
            } else {
                /* part (3) */

                /* clears the exhausted value option */
                tempBoard[StackTop->currentCoordinate.i][StackTop->currentCoordinate.j] = 0;

                /*decrease the available options amount left*/
                StackTop->possibleValuesCount--;

                /* marking ready for next child to be pushed*/
                StackTop->beforeChildPoped = true;

                /* for clarity - exa
                 * so the scope won't get destroyed, if there is more options
                 */
                continue;
            }
        }


        if (StackTop->isInitialized == true) {
            /* part 4 */
            StackFrame *parentScope = StackTop->parentScope;
            free(StackTop->possibleValues);
            free(StackTop);

            /* pop */
            StackTop = parentScope;
        }

    } while (StackTop != NULL);

    /* End */

    free(emptyCells);
    destroyBoard(tempBoard, g_gameDim);

    printPrompt(PNumSolutionsOutput, returnValue);
    return returnValue;
}
