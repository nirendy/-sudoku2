#include "lp_solver.h"
#include "main_aux.h"
#include "solver.h"

#include "gurobi_c2.h" /* TODO: REMOVE */
/*#include "gurobi_c.h"*/

#define MAX_VAR_NAME_LEN 30
#define MAX_CONST_NAME_LEN 100

/* TODO: change from log directory */

#define GUR_LOG_FILE "../log/gur"
#define GUR_LOG_FILE2 "../log/gur.lp"
#define GUR_NAME "sud"


/* LP-Solver module - responsible for LP nad ILP board solutions */

/*
 * Implementation details: there are module global variables of the GRenv and GRBmodel
 * The module is not interactive, ie. it will not return any kind of data structure to have further interact with it
 * That is why supporting only one model solving in a time (assuming only one GRBmodel) is allowed.
 * */

/* Gurobi module variables */

GRBenv *env = NULL;
GRBmodel *model = NULL;

/* Typedefs */

/* A structure represent a possible Xijk variable as presented in the Project instructions */
typedef struct _PossibleVar {
    /* DATA */
    char name[MAX_VAR_NAME_LEN]; /* the display name of the var*/
    int value; /* the possible value out of 1-N that this Node represent */
    int varIndex; /* the var index of the gurobi data structure */
    char type; /* whether it is Binary or Continues */
    double coeff; /* Coefficient of in Objective function */
    double prob; /* gurobi resulting probability of the value */

    /* Linked list implementation*/
    struct _PossibleVar *next;
} PossibleVar;

typedef struct _PossibleVarSentinel {
    PossibleVar *first;
    int length;
} PossibleVarSentinel;

/* Environment Constructor / Destructor */

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

/* Sudoku solver logic */

/* Utilities */
int calculateCoordinateFlatIndex(Coordinate coor) {
    /* convert a matrix index( tuple of 2 indexes) to a index of a flat array*/
    return (g_gameDim.N * coor.i) + coor.j;
}

Coordinate coordinateOfTheJCellInTheIBlock(int i, int j) {
    /* create the coordinate represents the J cell in the I block*/

    int n = g_gameDim.n;
    int m = g_gameDim.m;
    return createCoordinate(
            m * ((i * n) / (n * m)) + (j / n),
            ((i * n) % (n * m)) + (j % n)
    );
}

/* Supporting data structures Constructor / Destructor */
PossibleVar *createPossibleVar(Coordinate coor, int value, Bool isBinary) {
    PossibleVar *newPosVar = (PossibleVar *) smartMalloc(sizeof(PossibleVar));
    sprintf(newPosVar->name, "X_%d_%d_%d", coor.i + 1, coor.j + 1, value);
    newPosVar->value = value;

    newPosVar->varIndex = -1;
    newPosVar->type = isBinary ? GRB_BINARY : GRB_CONTINUOUS;
    newPosVar->coeff = isBinary ? 1.0 : randLimit(g_gameDim.N);
    newPosVar->prob = -1;
    newPosVar->next = NULL;

    return newPosVar;
}

PossibleVarSentinel *createCoor2Var(Board board, Bool isBinary) {
    int i;
    int emptyCellsCount;
    int *possibleValues;
    Coordinate *emptyCells;
    PossibleVarSentinel *coorV2var;

    /* TODO: test if 2 cells both has only one conflicting option*/
    /* Check the board isn't erroneous and all cells has at least one available option*/
    if (hasEmptyCellWithNoPossibleValues(board) == true || isBoardErroneous(board) == true) {
        return NULL;
    }

    coorV2var = (PossibleVarSentinel *) smartCalloc(g_gameDim.cellsCount, sizeof(PossibleVarSentinel));

    emptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    /*find empty cells*/
    emptyCellsCount = getEmptyCells(board, emptyCells);


    for (i = 0; i < emptyCellsCount; i++) {
        int possibleValuesCount;
        Coordinate currentCoordinate;
        PossibleVarSentinel *sentinel;

        /*create var for each possible values and cell*/
        currentCoordinate = emptyCells[i];
        possibleValues = (int *) smartMalloc(g_gameDim.N * sizeof(int));
        possibleValuesCount = getPossibleValues(board, currentCoordinate, possibleValues);
        sentinel = &coorV2var[calculateCoordinateFlatIndex(currentCoordinate)];
        sentinel->length = possibleValuesCount;

        /* TODO: remove - should never get here because checked before */
        if (possibleValuesCount == 0) {
            printf("Unreachable Code Error");
            exit(-1);
        }

        if (possibleValuesCount > 0) {
            PossibleVar *last;
            last = sentinel->first = createPossibleVar(currentCoordinate, possibleValues[0], isBinary);

            while (--possibleValuesCount > 0) {
                last = last->next = createPossibleVar(
                        currentCoordinate, possibleValues[sentinel->length - possibleValuesCount], isBinary
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

    if (coorV2var == NULL) {
        return;
    }

    /* free possible values nodes*/
    for (i = 0; i < g_gameDim.cellsCount; i++) {
        cur = coorV2var[i].first;

        /* free all nodes from the beginning to the end, stops when NULL node received*/
        while (cur != NULL) {
            temp = cur;
            cur = cur->next;
            free(temp);
        }
    }

    free(coorV2var);
}


/* GRB libray usage (using the above data structure)*/

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

FinishCode addVarsToModel(PossibleVarSentinel *coorV2var) {
    int error;
    int i;
    int numOfVarsCreated = 0;
    PossibleVar *curPosVar;


    for (i = 0; i < g_gameDim.cellsCount; i++) {
        /*create var for each possible values and cell*/
        if (coorV2var[i].length > 0) {
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

FinishCode addConstrainsToModel(PossibleVarSentinel *coorV2var) {
    int i, j, k;
    int error;
    int *constInd;
    double *constVals;
    int www;

    /*create equation for every value in every row, column and block*/

    constInd = (int *) smartMalloc(g_gameDim.N * sizeof(int));
    constVals = (double *) smartMalloc(g_gameDim.N * sizeof(double));

    for (i = 0; i < g_gameDim.N; i++) {
        constVals[i] = 1;
    }

    /* cells constraint*/

    for (i = 0; i < g_gameDim.N; i++) {
        for (j = 0; j < g_gameDim.N; j++) {
            int relvantVarsCount = 0;
            char constName[MAX_CONST_NAME_LEN];

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

    /* TODO: find better name*/
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
                exit(-1);
            }
        }

        for (k = 1; k <= g_gameDim.N; k++) {
            for (i = 0; i < g_gameDim.N; i++) {
                Bool hadObviousCell = false;
                int relvantVarsCount = 0;
                char constName[MAX_CONST_NAME_LEN];

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
                            exit(-1);
                        }
                    }

                    if (posVar != NULL) {
                        if (posVar->prob == 1) {
                            hadObviousCell = true;
                            break;
                        }
                        if (posVar->varIndex >= 0) {
                            constInd[relvantVarsCount] = posVar->varIndex;
                            relvantVarsCount++;
                        }
                    }

                }
                if (hadObviousCell == true) {
                    continue;
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
    if (coorV2var == NULL) {
        return FC_INVALID_RECOVERABLE;
    }

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
    if (coorV2var == NULL) {
        return FC_INVALID_RECOVERABLE;
    }

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

    coorV2var = createCoor2Var(board, false);
    if (coorV2var == NULL) {
        return FC_INVALID_RECOVERABLE;
    }

    finishCode = fillModel(coorV2var);
    if (finishCode != FC_SUCCESS) {
        return finishCode;
    }

    valsOptions = (int *) smartMalloc(g_gameDim.N * sizeof(int));

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
            if (bestOptionVal >= threshold) { /* TODO: test with thres=0 */
                board[i][j] = valsOptions[randLimit(bestOptionsCount)];
            }
        }
    }


    free(valsOptions);
    destroyCoorV2Var(coorV2var);

    return FC_SUCCESS;
}

/* Public Functions */

Bool fillSolutionMatrix(Board board, Board solutionBoard) {

    FinishCode finishCode;
    initGurobiEnv();
    copyBoard(solutionBoard, board);
    finishCode = fillBoard(solutionBoard);
    destroyGurobiEnv();
    /* TODO: assert the board is full and correct*/
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

Bool guessBoard(Board board, Board solutionBoard, double threshold) {
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
    return finishCode == FC_SUCCESS;
}

