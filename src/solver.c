#include <stdio.h>
#include "solver.h"

int getEmptyCells(Board board, Coordinate *emptyCells) {
    int i, j, emptyCount = 0;

    /* go over each cell of the matrix*/
    for (i = 0; i < N * M; ++i) {
        for (j = 0; j < N * M; ++j) {
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
    Coordinate emptyCells[N * N * M * M];
    int emptyCellsCount;

    emptyCellsCount = getEmptyCells(game->user_matrix, emptyCells);

    /* if 0 empty cells */
    return emptyCellsCount == 0;
}

void copyBoard(Board sourceBoard, Board targetBoard) {
    int i, j;
    for (i = 0; i < N * M; ++i) {
        for (j = 0; j < N * M; ++j) {
            targetBoard[i][j] = sourceBoard[i][j];
        }
    }
}

void clearBoard(Board board) {
    int i, j;
    for (i = 0; i < N * M; ++i) {
        for (j = 0; j < N * M; ++j) {
            board[i][j] = 0;
        }
    }
}

void coordinateNeighbours(Coordinate coordinate, Coordinate neighbours[3 * N * M - N - M + 1]) {
    int i, j, k, neighboursCreated = 0;

    /* find leftmost coordinate*/
    Coordinate leftMostBlockCoordinate = createCoordinate(
            coordinate.i - coordinate.i % N,
            coordinate.j - coordinate.j % M
    );

    /* go over all cell in the block*/
    for (i = leftMostBlockCoordinate.i + 0; i < leftMostBlockCoordinate.i + N; ++i) {
        for (j = leftMostBlockCoordinate.j + 0; j < leftMostBlockCoordinate.j + M; ++j) {
            if (i != coordinate.i && j != coordinate.j) {
                neighbours[neighboursCreated] = createCoordinate(i, j);
                neighboursCreated++;
            }
        }
    }

    /* go over all cells in the column and row expect of the one's in the block*/
    for (k = 0; k < N * M; ++k) {
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
    Coordinate neighbours[3 * N * M - N - M - 1];

    /* init an array of all numbers available*/
    for (i = 0; i < N * M; ++i) {
        possibleValues[i] = i + 1;
    }

    coordinateNeighbours(coordinate, neighbours);

    /* zeroing values from the array of used numbers*/
    for (i = 0; i < 3 * N * M - N - M - 1; ++i) {
        int neighbourValue = board[neighbours[i].i][neighbours[i].j];
        if (neighbourValue != 0) {
            possibleValues[neighbourValue - 1] = 0;
        }
    }

    /*closing the gap of zeroes*/
    /*for each value in array*/
    for (i = 0; i < N * M; ++i) {
        /* if it isn't zero*/
        if (possibleValues[i] != 0) {
            /*put the non zero value to the next actual filled cell*/
            possibleValues[possibleValuesCount] = possibleValues[i];
            possibleValuesCount++;
        }
    }

    /*zeroing the rest of the array - not necessary but cleaner*/
    for (i = possibleValuesCount; i < N * M; ++i) {
        possibleValues[i] = 0;
    }

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
    int possibleValues[N * M];

    Coordinate currentCoordinate = emptyCells[start];
    possibleValuesCount = getPossibleValues(board, currentCoordinate, possibleValues);

    /*if no options available*/
    if (possibleValuesCount == 0) {
        return false;
    }

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
            return true;
        }

        /* clears the unsuccessful cell guess*/
        board[currentCoordinate.i][currentCoordinate.j] = 0;

        /*decrease the available options amount left*/
        possibleValuesCount--;
    }

    return false;
}

/*solve the second parameter based on the first parameter*/
Bool solveBoard(Board board, Board solvedBoard, Bool isDeterministic) {
    Coordinate emptyCells[N * N * M * M];
    int emptyCellsCount;

    /* make a copy of the current board to solve*/
    copyBoard(board, solvedBoard);

    /*needed for the init of the recursion*/
    emptyCellsCount = getEmptyCells(board, emptyCells);

    /*solve the board cursively*/
    return solveBoardRec(solvedBoard, isDeterministic, emptyCells, emptyCellsCount, 0);
}

void generateFixedBoard(BoolBoard board, int fixedAmount) {
    int i, j, fixedCellsFound = 0;
    clearBoard((Board) board);

    while (fixedCellsFound < fixedAmount) {
        j = randLimit(N * N);
        i = randLimit(N * N);

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

    /* fill the fixed cells only*/
    for (i = 0; i < N * M; ++i) {
        for (j = 0; j < N * M; ++j) {
            if (game->fixed_matrix[i][j]) {
                game->user_matrix[i][j] = game->solved_matrix[i][j];
            }
        }
    }
}
