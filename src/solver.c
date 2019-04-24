#include <stdio.h>
#include "solver.h"
#include "parser.h"

/* Gurobi*/
#include "gurobi_c2.h"
/*#include "gurobi_c.h"*/


#define GUR_LOG_FILE "../log/gur"
#define GUR_LOG_FILE2 "../log/gur.lp"
#define GUR_NAME "sud"

GRBenv *env = NULL;
GRBmodel *model = NULL;


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

Bool isFullUserBoard(Game *game) {
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

Bool isPossibleValue(Board board, Coordinate coordinate, int value) {
    int *possibleValues;
    int possibleValuesCount;
    int i;
    Bool returnValue = false;

    possibleValues = (int *) malloc(g_gameDim.N * sizeof(int));

    possibleValuesCount = getPossibleValues(board, coordinate, possibleValues);
    for (i = 0; i < possibleValuesCount; i++) {
        if (possibleValues[i] == value) {
            returnValue = true;
            break;
        }
    }

    free(possibleValues);
    return returnValue;
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

int countPossibleSolutionsRec(Board board, Coordinate *emptyCells, int emptyCellsCount, int start) {
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

void old_generateGame(Game *game, int fixedAmount) {
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


/* Gurobi*/
FinishCode initGurobiEnv() {
    int error;

    error = GRBloadenv(&env, GUR_LOG_FILE);
    if (error) {
        printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
        /*TODO: is recoverable?*/
        return FC_INVALID_RECOVERABLE;
    }

    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
    }


    error = GRBnewmodel(env, &model, GUR_NAME, 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
        /*TODO: is recoverable?*/
        return FC_INVALID_RECOVERABLE;
    }


    return FC_SUCCESS;
}

void destroyGurobiEnv() {
    GRBfreemodel(model);
    GRBfreeenv(env);
}

typedef struct _PossibleVar {
    char name[10];
    int varIndex;
    char type;
    double coeff;
    double prob;
    Coordinate coordinate;
    int value;
    struct _PossibleVar *next;
} PossibleVar;

typedef struct _PossibleVarSentinel {
    PossibleVar *first;
    int length;
} PossibleVarSentinel;


int calculateCoordinateFlatIndex(Coordinate coor) {
    return (g_gameDim.N * coor.i) + coor.j;
}

PossibleVar *getPossibleVarFromCoor2Var(PossibleVarSentinel *coorV2var, Coordinate coor, int value) {
    PossibleVar *posVar;
    posVar = coorV2var[calculateCoordinateFlatIndex(coor)].first;
    while (posVar != NULL) {
        if (posVar->value == value) {
            return posVar;
        }
        posVar = posVar->next;
    }
    return NULL;
}

PossibleVar *createPossibleVar(Coordinate coor, int value, Bool isBinary, Bool isOnlyOption) {
    PossibleVar *newPosVar = (PossibleVar *) malloc(sizeof(PossibleVar));
    sprintf(newPosVar->name, "X_%d_%d_%d", coor.i + 1, coor.j + 1, value);
    newPosVar->varIndex = -1;
    newPosVar->type = isBinary ? GRB_BINARY : GRB_CONTINUOUS;
    newPosVar->coeff = isBinary ? 1.0 : randLimit(g_gameDim.N);
    newPosVar->prob = isOnlyOption == true ? 1 : -1;
    newPosVar->coordinate = coor;
    newPosVar->value = value;
    newPosVar->next = NULL;

    return newPosVar;
}

PossibleVarSentinel *createCoor2Var(Board board, Bool isBinary) {
    int i;
    int emptyCellsCount;
    int *possibleValues;
    Coordinate *emptyCells;
    PossibleVarSentinel *coorV2var;

    coorV2var = (PossibleVarSentinel *) calloc(g_gameDim.cellsCount, sizeof(PossibleVar));
    emptyCells = (Coordinate *) malloc(g_gameDim.cellsCount * sizeof(Coordinate));
    /*find empty cells*/
    emptyCellsCount = getEmptyCells(board, emptyCells);


    for (i = 0; i < emptyCellsCount; i++) {
        int possibleValuesCount;
        Coordinate currentCoordinate;
        PossibleVarSentinel *sentinel;

        /*create var for each possible values and cell*/
        currentCoordinate = emptyCells[i];
        possibleValues = (int *) malloc(g_gameDim.N * sizeof(int));
        possibleValuesCount = getPossibleValues(board, currentCoordinate, possibleValues);
        sentinel = &coorV2var[calculateCoordinateFlatIndex(currentCoordinate)];
        sentinel->length = possibleValuesCount;

        if (possibleValuesCount > 0) {
            PossibleVar *last;
            last = sentinel->first = createPossibleVar(currentCoordinate, possibleValues[0], isBinary,
                                                       sentinel->length == 1);

            while (--possibleValuesCount > 0) {
                last = last->next = createPossibleVar(
                        currentCoordinate, possibleValues[sentinel->length - possibleValuesCount], isBinary, false
                );
            }
        }
        free(possibleValues);
    }

    free(emptyCells);

    return coorV2var;
}

void destroyCoorV2Var(PossibleVarSentinel *coorV2var) {
    int i;
    PossibleVar *cur, *temp;

    for (i = 0; i < g_gameDim.cellsCount; ++i) {
        cur = coorV2var[i].first;
        while (cur != NULL) {
            temp = cur;
            cur = cur->next;
            free(temp);
        }
    }

    free(coorV2var);
}


FinishCode addVarsToModel(PossibleVarSentinel *coorV2var) {
    int error;
    int i;
    int numOfVarsCreated = 0;
    PossibleVar *curPosVar;


    for (i = 0; i < g_gameDim.cellsCount; i++) {
        /*create var for each possible values and cell*/
        if (coorV2var[i].length > 1) {
            curPosVar = coorV2var[i].first;
            while (curPosVar != NULL) {
                curPosVar->varIndex = numOfVarsCreated;

                /* add variables to model */
                error = GRBaddvar(model, 0, NULL, NULL, curPosVar->coeff, 0.0, 1.0, curPosVar->type, curPosVar->name);
                if (error) {
                    printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
                    return FC_INVALID_RECOVERABLE;
                }

                numOfVarsCreated++;
                curPosVar = curPosVar->next;
            }
        }
    }

    /* Change objective sense to maximization */
    error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    /* update the model - to integrate new variables */

    error = GRBupdatemodel(model);
    if (error) {
        printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
        return FC_INVALID_RECOVERABLE;
    }

    return FC_SUCCESS;
}

Coordinate coordinateOfTheJCellInTheIBlock(int i, int j) {
    int n = g_gameDim.n;
    int m = g_gameDim.m;
    return createCoordinate(
            m * ((i * n) / (n * m)) + (j / n),
            ((i * n) % (n * m)) + (j % n)
    );
}

FinishCode addConstrainsToModel(PossibleVarSentinel *coorV2var) {
    int i, j, k;
    int error;
    int *constInd;
    double *constVals;
    int www;

    /*create equation for every value in every row, column and block*/

    constInd = (int *) malloc(g_gameDim.N * sizeof(int));
    constVals = (double *) malloc(g_gameDim.N * sizeof(double));

    for (i = 0; i < g_gameDim.N; i++) {
        constVals[i] = 1;
    }

    /* cells constraint*/

    for (i = 0; i < g_gameDim.N; i++) {
        for (j = 0; j < g_gameDim.N; j++) {
            int relvantVarsCount = 0;
            char constName[100];

            for (k = 1; k <= g_gameDim.N; k++) {
                PossibleVar *posVar;
                posVar = getPossibleVarFromCoor2Var(coorV2var, createCoordinate(i, j), k);

                if (posVar != NULL && posVar->varIndex >= 0) {
                    constInd[relvantVarsCount] = posVar->varIndex;
                    relvantVarsCount++;
                }
            }

            if (relvantVarsCount > 1) {
                sprintf(constName, "values of cell (%d,%d)", i + 1, j + 1);

                /* add constraint to model*/
                error = GRBaddconstr(model, relvantVarsCount, constInd, constVals, GRB_EQUAL, 1.0, constName);
                if (error) {
                    printf("ERROR %d GRBaddconstr(%s): %s\n", error, constName, GRBgeterrormsg(env));
                    /*return FC_INVALID_RECOVERABLE;*/ /*TODO: add this?*/
                } else {
                    /*printf("GRBaddconstr(%s)\n", constName);*/  /*TODO: delete*/
                }
            }
        }
    }

    for (www = 0; www <= 2; www++) {
        char wwwName[10];
        switch (www) {
            case 0: {
                /* rows */
                sprintf(wwwName, "%s", "row");
                break;
            }
            case 1: {
                /* columns */
                sprintf(wwwName, "%s", "column");
                break;
            }
            case 2: {
                /* blocks */
                sprintf(wwwName, "%s", "block");
                break;
            }
            default: {
                printf("Unreachable Code Error");
                exit(FC_UNEXPECTED_ERROR);
            }
        }

        for (k = 1; k <= g_gameDim.N; k++) {
            for (i = 0; i < g_gameDim.N; i++) {
                int relvantVarsCount = 0;
                char constName[100];

                for (j = 0; j < g_gameDim.N; j++) {
                    PossibleVar *posVar;

                    switch (www) {
                        case 0: {
                            /* rows */
                            posVar = getPossibleVarFromCoor2Var(coorV2var, createCoordinate(i, j), k);
                            break;
                        }
                        case 1: {
                            /* columns */
                            posVar = getPossibleVarFromCoor2Var(coorV2var, createCoordinate(j, i), k);
                            break;
                        }
                        case 2: {
                            /* blocks */
                            posVar = getPossibleVarFromCoor2Var(coorV2var, coordinateOfTheJCellInTheIBlock(i, j), k);
                            break;
                        }
                        default: {
                            printf("Unreachable Code Error");
                            exit(FC_UNEXPECTED_ERROR);
                        }
                    }

                    if (posVar != NULL && posVar->varIndex >= 0) {
                        constInd[relvantVarsCount] = posVar->varIndex;
                        relvantVarsCount++;
                    }

                }

                if (relvantVarsCount > 1) {
                    sprintf(constName, "%s #%d with value of %d", wwwName, i + 1, k);

                    /* add constraint to model*/
                    error = GRBaddconstr(model, relvantVarsCount, constInd, constVals, GRB_EQUAL, 1.0, constName);
                    if (error) {
                        printf("ERROR %d GRBaddconstr(%s): %s\n", error, constName, GRBgeterrormsg(env));
                        for (j = 0; j < relvantVarsCount; j++) { printf("%d,", constInd[j]); }
                        printf("\n");
                        /*return FC_INVALID_RECOVERABLE;*/ /*TODO: add this?*/
                    } else {
                        /*printf("GRBaddconstr(%s):\n", constName);*/ /*TODO: delete*/
                    }

                }

            }
        }
    }

    free(constInd);
    free(constVals);

    return FC_SUCCESS;
}

FinishCode optimizeModel() {
    int error;
    int optimstatus;

    /* Optimize model - need to call this before calculation */
    error = GRBoptimize(model);
    if (error) {
        printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
        return FC_INVALID_RECOVERABLE;
    }

    /* Write model to 'mip1.lp' - this is not necessary but very helpful */
    error = GRBwrite(model, GUR_LOG_FILE2);
    if (error) {
        printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
        return FC_INVALID_RECOVERABLE;
    }

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
        return FC_INVALID_RECOVERABLE;
    }

    if (optimstatus == GRB_OPTIMAL) { /* solution found */
        /*printf("Solution found\n"); */ /*TODO: should print something?*/
        return FC_SUCCESS;
    } else if (optimstatus == GRB_INF_OR_UNBD ||
               optimstatus == GRB_INFEASIBLE ||
               optimstatus == GRB_UNBOUNDED) { /* no solution found */
        /*printf("Model is infeasible or unbounded\n");*/ /*TODO: should print something?*/
        return FC_SUCCESS_NOT_SOLVABLE;
    } else { /* error or calculation stopped */
        printf("Optimization was stopped early, status: %d\n", optimstatus);
        return FC_INVALID_RECOVERABLE;
    }
}

FinishCode fillModel(PossibleVarSentinel *coorV2var) {
    FinishCode finishCode;

    finishCode = addVarsToModel(coorV2var);
    if (finishCode != FC_SUCCESS) {
        return finishCode;
    }

    finishCode = addConstrainsToModel(coorV2var);
    if (finishCode != FC_SUCCESS) {
        return finishCode;
    }


    return (optimizeModel());
}

FinishCode fillBoard(Board board) {
    int error;
    FinishCode finishCode;
    PossibleVarSentinel *coorV2var; /* for the possibleVars[] */
    int i, j, k;

    coorV2var = createCoor2Var(board, true);
    finishCode = fillModel(coorV2var);
    if (finishCode != FC_SUCCESS) {
        return finishCode;
    }


    /*get solutions*/
    for (i = 0; i < g_gameDim.N; i++) {
        for (j = 0; j < g_gameDim.N; j++) {
            for (k = 1; k <= g_gameDim.N; k++) {
                PossibleVar *posVar = getPossibleVarFromCoor2Var(coorV2var, createCoordinate(i, j), k);
                if (posVar != NULL) {
                    if (posVar->varIndex >= 0) {
                        error = GRBgetdblattrelement(model, GRB_DBL_ATTR_X, posVar->varIndex, &posVar->prob);

                        if (error) {
                            printf("ERROR %d GRBgetdblattrelement(%s): %s\n", error, posVar->name, GRBgeterrormsg(env));
                            /*return FC_INVALID_RECOVERABLE;*/ /*TODO: add this?*/
                        } else {
                            /*printf("GRBgetdblattrelement(%s)\n", posVar->name);*/  /*TODO: delete*/
                        }
                    }

                    if (posVar->prob == 1) {
                        board[i][j] = k;
                    }
                }
            }
        }
    }

    destroyCoorV2Var(coorV2var);

    return FC_SUCCESS;
}

FinishCode guessFillBoardAndGuessHint(Board board, Coordinate coor) {
    int error;
    FinishCode finishCode;
    PossibleVarSentinel *coorV2var; /* for the possibleVars[] */
    int k;

    coorV2var = createCoor2Var(board, false);

    finishCode = fillModel(coorV2var);
    if (finishCode != FC_SUCCESS) {
        return finishCode;
    }

    for (k = 1; k <= g_gameDim.N; k++) {
        PossibleVar *posVar = getPossibleVarFromCoor2Var(coorV2var, coor, k);
        if (posVar != NULL) {
            if (posVar->varIndex >= 0) {
                error = GRBgetdblattrelement(model, GRB_DBL_ATTR_X, posVar->varIndex, &posVar->prob);

                if (error) {
                    printf("ERROR %d GRBgetdblattrelement(): %s\n", error, GRBgeterrormsg(env));
                    return FC_INVALID_RECOVERABLE;
                }
            }

            if (posVar->prob > 0) {
                printf("%d -> %f\n", k, posVar->prob);
            }
        }
    }

    destroyCoorV2Var(coorV2var);

    return FC_SUCCESS;
}

FinishCode guessFillBoard(Board board, double threshold) {
    int error;
    FinishCode finishCode;
    PossibleVarSentinel *coorV2var; /* for the possibleVars[] */
    int i, j, k;
    int *valsOptions;
    int bestOptionsCount;
    double bestOptionVal;

    valsOptions = (int *) malloc(g_gameDim.N * sizeof(int));

    coorV2var = createCoor2Var(board, false);

    finishCode = fillModel(coorV2var);
    if (finishCode != FC_SUCCESS) {
        return finishCode;
    }

    for (i = 0; i < g_gameDim.N; i++) {
        for (j = 0; j < g_gameDim.N; j++) {
            bestOptionsCount = 0;
            bestOptionVal = 0;

            for (k = 1; k <= g_gameDim.N; k++) {
                PossibleVar *posVar = getPossibleVarFromCoor2Var(coorV2var, createCoordinate(i, j), k);
                if (posVar != NULL) {
                    if (posVar->varIndex >= 0) {
                        error = GRBgetdblattrelement(model, GRB_DBL_ATTR_X, posVar->varIndex, &posVar->prob);

                        if (error) {
                            printf("ERROR %d GRBgetdblattrelement(): %s\n", error, GRBgeterrormsg(env));
                            return FC_INVALID_RECOVERABLE;
                        }
                    }

                    if (posVar->prob >= bestOptionVal && isPossibleValue(board, createCoordinate(i, j), k)) {
                        if (posVar->prob > bestOptionVal) {
                            bestOptionVal = posVar->prob;
                            bestOptionsCount = 0;
                        }

                        valsOptions[bestOptionsCount] = k;
                        bestOptionsCount++;
                    }
                }
            }
            if (bestOptionVal >= threshold) {
                board[i][j] = valsOptions[randLimit(bestOptionsCount)];
            }
        }
    }


    free(valsOptions);
    destroyCoorV2Var(coorV2var);

    return FC_SUCCESS;
}

Bool fillSolutionMatrix(Board board, Board solutionBoard) {

    FinishCode finishCode;
    initGurobiEnv();
    copyBoard(solutionBoard, board);
    finishCode = fillBoard(solutionBoard);
    /*printUserBoard(solutionBoard);*/ /* TODO: REMOVE*/
    destroyGurobiEnv();
    return finishCode == FC_SUCCESS;
}

void guessHint(Board board, Coordinate coordinate) {
    FinishCode finishCode;
    Board tempBoard = createBoard();
    copyBoard(tempBoard, board);

    finishCode = initGurobiEnv();
    if (finishCode == FC_SUCCESS) {
        finishCode = guessFillBoardAndGuessHint(tempBoard, coordinate);
    }

    if (finishCode != FC_SUCCESS) {
        printf("Could not guess hint\n"); /*TODO: better print*/
    }

    destroyBoard(tempBoard, g_gameDim);
    destroyGurobiEnv();
}



void guessBoard(Board board, Board solutionBoard, double threshold) {
    FinishCode finishCode;
    copyBoard(solutionBoard, board);

    finishCode = initGurobiEnv();
    if (finishCode == FC_SUCCESS) {
        finishCode = guessFillBoard(solutionBoard, threshold);
    }

    if (finishCode != FC_SUCCESS) {
        printf("Could not guess fill board\n"); /*TODO: better print*/
    }

    destroyGurobiEnv();
}

