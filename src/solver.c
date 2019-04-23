#include <stdio.h>
#include "solver.h"
#include "parser.h"

/* Gurobi*/
#include "gurobi_c.h"

GRBenv *env = NULL;
int error = 0;
Bool isGurobiEnvInitialized = false;

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

Bool isSolved(Game *game) {
    Coordinate *emptyCells;
    int emptyCellsCount;

    emptyCells = (Coordinate *) malloc(g_gameDim.cellsCount * sizeof(Coordinate));

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

void coordinateNeighbours(Coordinate coordinate, Coordinate *neighbours) {
    int i, j, k, neighboursCreated = 0;

    /* find leftmost coordinate*/
    Coordinate leftMostBlockCoordinate = createCoordinate(
            coordinate.i - (coordinate.i % g_gameDim.m),
            coordinate.j - (coordinate.j % g_gameDim.n)
    );

    /* go over all cell in the block*/
    for (i = leftMostBlockCoordinate.i + 0; i < leftMostBlockCoordinate.i + g_gameDim.m; ++i) {
        for (j = leftMostBlockCoordinate.j + 0; j < leftMostBlockCoordinate.j + g_gameDim.n; ++j) {
            if (i != coordinate.i && j != coordinate.j) {
                neighbours[neighboursCreated] = createCoordinate(i, j);
                neighboursCreated++;
            }
        }
    }

    /* go over all cells in the column and row expect of the one's in the block*/
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

int getPossibleValues(Board board, Coordinate coordinate, int *possibleValues) {
    int i, possibleValuesCount = 0;
    Coordinate *neighbours;
    neighbours = (Coordinate *) malloc(g_gameDim.cellNeighboursCount * sizeof(Coordinate));

    /* init an array of all numbers available*/
    for (i = 0; i < g_gameDim.N; ++i) {
        possibleValues[i] = i + 1;
    }

    coordinateNeighbours(coordinate, neighbours);

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

/* return the removed value */
int removeArrayIndex(int *arr, int arrLength, int removeI) {
    int i, removedValue = arr[removeI];

    /*closing the gap of the removed index*/
    for (i = removeI; i < arrLength - 1; ++i) {
        arr[i] = arr[i + 1];
    }

    return removedValue;
}

/* return the removed value randomly*/
int randomRemoveArrayIndex(int *arr, int arrLength) {
    return removeArrayIndex(arr, arrLength, randLimit(arrLength));
}

Bool solveBoardRec(Board board, Bool isDeterministic, Coordinate *emptyCells, int emptyCellsCount, int start) {
    int possibleValuesCount;
    int *possibleValues;
    Bool returnValue = false;
    Coordinate currentCoordinate = emptyCells[start];

    possibleValues = (int *) malloc(g_gameDim.N * sizeof(int));

    possibleValuesCount = getPossibleValues(board, currentCoordinate, possibleValues);

    /*if any option available*/
    if (possibleValuesCount > 0) {
        /* as long there are more possible values that we didn't check*/
        while (possibleValuesCount > 0) {
            int nextValue;

            /*if it is deterministic or there are only one option left*/
            if (possibleValuesCount == 1 || isDeterministic) {
                /* remove the lowest option*/
                nextValue = removeArrayIndex(possibleValues, possibleValuesCount, 0);
            } else {
                /* remove a random option*/
                nextValue = randomRemoveArrayIndex(possibleValues, possibleValuesCount);
            }

            /*change the value to the next value*/
            board[currentCoordinate.i][currentCoordinate.j] = nextValue;

            /* if this is the last cell
             * OR
             * if this configuration leads to a winning configuration
             * */
            if ((start == emptyCellsCount - 1) ||
                (solveBoardRec(board, isDeterministic, emptyCells, emptyCellsCount, start + 1))) {
                returnValue = true;
                break;
            }

            /* clears the unsuccessful cell guess*/
            board[currentCoordinate.i][currentCoordinate.j] = 0;

            /*decrease the available options amount left*/
            possibleValuesCount--;
        }
    }

    free(possibleValues);
    return returnValue;
}

int
countPossibleSolutionsRec(Board board, Coordinate *emptyCells, int emptyCellsCount, int start) {
    int possibleValuesCount;
    int *possibleValues;
    int returnValue = 0;
    Coordinate currentCoordinate = emptyCells[start];

    possibleValues = (int *) malloc(g_gameDim.N * sizeof(int));

    possibleValuesCount = getPossibleValues(board, currentCoordinate, possibleValues);

    /*if any option available*/
    if (possibleValuesCount > 0) {
        /* as long there are more possible values that we didn't check*/
        while (possibleValuesCount > 0) {
            int nextValue;

            /* remove the lowest option*/
            nextValue = removeArrayIndex(possibleValues, possibleValuesCount, 0);

            /*change the value to the next value*/
            board[currentCoordinate.i][currentCoordinate.j] = nextValue;

            /* if this is the last cell
             * OR
             * if this configuration leads to a winning configuration
             * */
            if (start == emptyCellsCount - 1) {
                returnValue += 1;
            } else {
                returnValue += countPossibleSolutionsRec(board, emptyCells, emptyCellsCount, start + 1);
            }



            /* clears the unsuccessful cell guess*/
            board[currentCoordinate.i][currentCoordinate.j] = 0;

            /*decrease the available options amount left*/
            possibleValuesCount--;
        }
    }

    free(possibleValues);
    return returnValue;
}

/*solve the second parameter based on the first parameter*/
Bool solveBoard(Board userBoard, Board toSolveBoard, Bool isDeterministic) {
    Coordinate *emptyCells;
    int emptyCellsCount;
    Bool returnValue;

    emptyCells = (Coordinate *) malloc(g_gameDim.cellsCount * sizeof(Coordinate));

    /* make a copy of the current board to solve*/
    copyBoard(toSolveBoard, userBoard);

    /*needed for the init of the recursion*/
    emptyCellsCount = getEmptyCells(userBoard, emptyCells);

    /*solve the board cursively*/
    returnValue = solveBoardRec(toSolveBoard, isDeterministic, emptyCells, emptyCellsCount, 0);
    free(emptyCells);
    return returnValue;
}

typedef struct _CountPossibleSolutionsScope {
    int start;
    Coordinate currentCoordinate;
    int *possibleValues;
    int possibleValuesCount;
    int nextValue;
    Bool beforeChildPoped;
    Bool isInitialized;
    struct _CountPossibleSolutionsScope *parentScope;
} CountPossibleSolutionsScope;

FinishCode countPossibleSolutions(Board board) {
    Coordinate *emptyCells;
    int emptyCellsCount;
    int returnValue = 0;
    Board tempBoard;
    CountPossibleSolutionsScope *curScope;

    emptyCells = (Coordinate *) malloc(g_gameDim.cellsCount * sizeof(Coordinate));
    tempBoard = createBoard();

    /* make a copy of the current board to solve*/
    copyBoard(tempBoard, board);

    /*needed for the init of the recursion*/
    emptyCellsCount = getEmptyCells(tempBoard, emptyCells);


    curScope = (CountPossibleSolutionsScope *) malloc(sizeof(CountPossibleSolutionsScope));
    curScope->parentScope = NULL;
    curScope->beforeChildPoped = true;
    curScope->isInitialized = false;
    curScope->start = 0;

    /* Begin */
    /*returnValue = countPossibleSolutionsRec(tempBoard, emptyCells, emptyCellsCount, 0);*/
    do {
        if (curScope->isInitialized == false) {
            /* initialize*/
            curScope->currentCoordinate = emptyCells[curScope->start];
            curScope->possibleValues = (int *) malloc(g_gameDim.N * sizeof(int));
            curScope->possibleValuesCount = getPossibleValues(tempBoard, curScope->currentCoordinate,
                                                              curScope->possibleValues);
            curScope->isInitialized = true;
        }

        /* as long there are more possible values that we didn't check*/
        while (curScope->possibleValuesCount > 0) {
            if (curScope->beforeChildPoped == true) {
                /* remove the lowest option*/
                curScope->nextValue = removeArrayIndex(curScope->possibleValues, curScope->possibleValuesCount, 0);

                /*change the value to the next value*/
                tempBoard[curScope->currentCoordinate.i][curScope->currentCoordinate.j] = curScope->nextValue;

                /* if this is the last cell
                 * */
                if (curScope->start == emptyCellsCount - 1) {
                    /*pop*/
                    returnValue += 1;
                    tempBoard[curScope->currentCoordinate.i][curScope->currentCoordinate.j] = 0;

                    /*if got to here, the value must be the last than no need to go over to next child*/
                    break;
                } else {
                    /*push*/

                    CountPossibleSolutionsScope *newScope =
                            (CountPossibleSolutionsScope *) malloc(sizeof(CountPossibleSolutionsScope));
                    newScope->parentScope = curScope;
                    newScope->beforeChildPoped = true;
                    newScope->isInitialized = false;
                    newScope->start = curScope->start + 1;

                    curScope->beforeChildPoped = false;
                    curScope = newScope;

                    /* so the new scope will be initialized*/
                    break;
                }
            } else {
                /* clears the unsuccessful cell guess*/
                tempBoard[curScope->currentCoordinate.i][curScope->currentCoordinate.j] = 0;
                /*decrease the available options amount left*/
                curScope->possibleValuesCount--;

                /* get ready for next child to be pushed*/
                curScope->beforeChildPoped = true;

                /* so the scope won't get destroyed, if there is more options, */
                continue;
            }
        }

        if (curScope->isInitialized == true) {
            /* pop */
            CountPossibleSolutionsScope *parentScope = curScope->parentScope;
            free(curScope->possibleValues);
            free(curScope);
            curScope = parentScope;
        }

    } while (curScope != NULL);

    /* End */

    free(emptyCells);
    destroyBoard(tempBoard, g_gameDim);

    printPrompt(PNumSolutionsOutput, returnValue);
    return returnValue;
}

void generateFixedBoard(BoolBoard board, int fixedAmount) {
    int i, j, fixedCellsFound = 0;
    clearBoard((Board) board);

    while (fixedCellsFound < fixedAmount) {
        j = randLimit(g_gameDim.N);
        i = randLimit(g_gameDim.N);

        if (board[i][j] == 0) {
            board[i][j] = 1;
            fixedCellsFound++;
        }
    }
}

void generateGame(Game *game, int fixedAmount) {
    int i, j;

    /*init the user board*/
    clearBoard(game->user_matrix);

    /*solve the board randomly*/
    solveBoard(game->user_matrix, game->solved_matrix, false);

    generateFixedBoard(game->fixed_matrix, fixedAmount);
    clearBoolBoard(game->error_matrix);

    /* fill the fixed cells only*/
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            if (game->fixed_matrix[i][j]) {
                game->user_matrix[i][j] = game->solved_matrix[i][j];
            }
        }
    }
}


void updateAfterClearErrorMatrix(Game *game, Input input) {
    int k;
    Input in;
    Coordinate cor;
    Coordinate *neighbours;
    neighbours = (Coordinate *) malloc(g_gameDim.cellNeighboursCount * sizeof(Coordinate));
    coordinateNeighbours(input.coordinate, neighbours);

    for (k = 0; k < g_gameDim.cellNeighboursCount; k++) {
        game->error_matrix[neighbours[k].i][neighbours[k].j] = 0;
    }

    for (k = 0; k < g_gameDim.cellNeighboursCount; k++) {
        cor.i = neighbours[k].i;
        cor.j = neighbours[k].j;
        in.coordinate = cor;
        in.value = game->user_matrix[cor.i][cor.j];
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
    neighbours = (Coordinate *) malloc(g_gameDim.cellNeighboursCount * sizeof(Coordinate));
    coordinateNeighbours(input.coordinate, neighbours);
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
    int i, j , val;
    Input input;
    Coordinate cor;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            cor.i = i;
            cor.j = j;
            val = game->user_matrix[i][j];
            input.coordinate = cor;
            input.value = val;

            if(val!=0){
                updateAfterSetErrorMatrix(game , input);
            }
        }
    }
}


/* Gurobi*/
void initGurobi() {

}

