#include "lp_solver.h"
#include "main_aux.h"
#include "solver.h"
#include "gurobi_c.h"

#define MAX_VAR_NAME_LEN 30
#define MAX_CONST_NAME_LEN 100
#define RANDOM_PRECISION 100

#define GUR_LOG_FILE "./gur.log"
#define GUR_LOG_FILE2 "./gur.lp"
#define GUR_NAME "sudoku"


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

typedef struct _WeightedValue {
    int value;
    double weight;
} WeightedValue;

/* Environment Constructor / Destructor */
Bool initGurobiEnv() {
    int error;

    /* so if it fails it can be always destroyed without pointing old address*/
    env = NULL;
    model = NULL;

    error = GRBloadenv(&env, GUR_LOG_FILE);
    if (error) {
        printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
        return false;
    }

    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
    }


    error = GRBnewmodel(env, &model, GUR_NAME, 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
        return false;
    }

    return true;
}

void destroyGurobiEnv() {
    GRBfreemodel(model);
    GRBfreeenv(env);
}

/* Sudoku solver logic */

/* Utilities */
/**
 * convert a matrix index( tuple of 2 indexes) to a index of a flat array
 * @param coor
 * @return index of a flat array
 */
int calculateCoordinateFlatIndex(Coordinate coor) {
    return (g_gameDim.N * coor.i) + coor.j;
}

/**
 * create the coordinate represents the J cell in the I block
 * @param i
 * @param j
 * @return the coordinate
 */
Coordinate coordinateOfTheJCellInTheIBlock(int i, int j) {
    int n = g_gameDim.n;
    int m = g_gameDim.m;

    return createCoordinate(
            m * ((i * n) / (n * m)) + (j / n),
            ((i * n) % (n * m)) + (j % n)
    );
}

/**
 * This function returns a int that is the chosen random value
 * @param weightedValues array of tuples that we want to make weighted random choose of the value
 * @param count number of tuples in the array
 * @return
 */
int getRandomWeightedValue(WeightedValue *weightedValues, int count) {
    double sumValues = 0;
    int i;
    double randomPoint;

    /* sum weights */
    for (i = 0; i < count; ++i) {
        sumValues += weightedValues[i].weight;
    }


    /* normalize weights*/
    if (sumValues == 0) {
        for (i = 0; i < count; ++i) {
            weightedValues[i].weight = (double) 1 / count;
        }
    } else {
        for (i = 0; i < count; ++i) {
            weightedValues[i].weight = weightedValues[i].weight / sumValues;
        }
    }

    /* choose random weighted value */
    /* get random point in [0,1] */
    randomPoint = (double) randLimit(RANDOM_PRECISION + 1) / RANDOM_PRECISION;


    /* decrease the weight of the value, stops when the value is non-positive */
    for (i = 0; i < count; ++i) {
        randomPoint -= weightedValues[i].weight;
        if (randomPoint <= 0) {
            return weightedValues[i].value;
        }
    }


    /* in case for some reason the floating point at the end didn't sum up to 1 (floating point precision issues) */
    return weightedValues[count - 1].value;
}

/* Supporting data structures Constructor / Destructor */

/**
 * Constructor of a Node of coor2var
 * @param coor
 * @param value
 * @param isBinary changes the the configuration to CONTINUES or BINARY, and coeff to objective (random or 1) accordingly
 * @return
 */
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

/**
 * consturctor to coor2var
 * @param board
 * @param isBinary
 * @return
 */
PossibleVarSentinel *createCoor2Var(Board board, Bool isBinary) {
    PossibleVarSentinel *coor2Var;
    Coordinate *emptyCells;
    int emptyCellsCount;
    int *possibleValues;
    int i;

    /* Check the board isn't erroneous and all cells has at least one available option*/
    if (hasEmptyCellWithNoPossibleValues(board) == true || isBoardErroneous(board) == true) {
        return NULL;
    }

    /* malloc a sentinel for every cell in the matrix */
    coor2Var = (PossibleVarSentinel *) smartCalloc(g_gameDim.cellsCount, sizeof(PossibleVarSentinel));

    /*find empty cells*/
    emptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    emptyCellsCount = getEmptyCells(board, emptyCells);
    possibleValues = (int *) smartMalloc(g_gameDim.N * sizeof(int));


    for (i = 0; i < emptyCellsCount; i++) {
        int possibleValuesCount;
        Coordinate currentCoordinate;
        PossibleVarSentinel *sentinel;

        /*create var for each possible values and cell*/
        currentCoordinate = emptyCells[i];
        possibleValuesCount = getPossibleValues(board, currentCoordinate, possibleValues);
        sentinel = &coor2Var[calculateCoordinateFlatIndex(currentCoordinate)];
        sentinel->length = possibleValuesCount;

        if (possibleValuesCount > 0) {
            int j;
            PossibleVar *lastNode;

            sentinel->first = createPossibleVar(currentCoordinate, possibleValues[0], isBinary);
            lastNode = sentinel->first;

            for (j = 1; j < possibleValuesCount; j++) {
                lastNode->next = createPossibleVar(currentCoordinate, possibleValues[j], isBinary);
                lastNode = lastNode->next;
            }
        } else {
            printError(EUnreachableCode);
        }
    }

    free(possibleValues);
    free(emptyCells);

    return coor2Var;
}

void destroyCoor2Var(PossibleVarSentinel *coor2Var) {
    int i;
    PossibleVar *cur, *temp;

    if (coor2Var == NULL) {
        return;
    }

    /* free possible values nodes*/
    for (i = 0; i < g_gameDim.cellsCount; i++) {
        cur = coor2Var[i].first;

        /* free all nodes from the beginning to the end, stops when NULL node received*/
        while (cur != NULL) {
            temp = cur;
            cur = cur->next;
            free(temp);
        }
    }

    /* free sentinels array*/
    free(coor2Var);
}

/* Data structure static methods */

PossibleVar *getPossibleVarFromCoor2Var(PossibleVarSentinel *coor2Var, Coordinate coor, int value) {
    PossibleVar *posVar;
    posVar = coor2Var[calculateCoordinateFlatIndex(coor)].first;

    /* search the value in all possible options of the coordinate */
    while (posVar != NULL) {
        if (posVar->value == value) {
            return posVar;
        }
        posVar = posVar->next;
    }
    return NULL;
}

/* GRB libray usage (using the above data structure)
 *
 * these function return a Boolean specifies if the command successfully completed
 * */

Bool addVarsToModel(PossibleVarSentinel *coor2Var) {
    int error;
    int i;
    int numOfVarsCreated = 0;
    PossibleVar *curPosVar;

    for (i = 0; i < g_gameDim.cellsCount; i++) {
        /*create var for each possible values and cell*/
        if (coor2Var[i].length > 0) {
            curPosVar = coor2Var[i].first;
            while (curPosVar != NULL) {

                /* associate gurubi refrence to the Ndde*/
                curPosVar->varIndex = numOfVarsCreated;

                /* add variables to model */
                error = GRBaddvar(model, 0, NULL, NULL, curPosVar->coeff, 0.0, 1.0, curPosVar->type, curPosVar->name);
                if (error) {
                    printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
                    return false;
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
        return false;
    }

    /* update the model - to integrate new variables */
    error = GRBupdatemodel(model);
    if (error) {
        printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
        return false;
    }

    return true;
}

Bool addConstrainsToModel(PossibleVarSentinel *coor2Var) {
    int i, j, k;
    int error;
    int *constInd;
    double *constVals;
    int neighbourType; /* 0: row, 1: column, 2: block*/
    Bool noErrors = true;

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
                posVar = getPossibleVarFromCoor2Var(coor2Var, createCoordinate(i, j), k);

                if (posVar != NULL && posVar->varIndex >= 0) {
                    constInd[relvantVarsCount] = posVar->varIndex;
                    relvantVarsCount++;
                }
            }

            if (relvantVarsCount > 0) {
                sprintf(constName, "values of cell (%d,%d)", i + 1, j + 1);

                /* add constraint to model*/
                error = GRBaddconstr(model, relvantVarsCount, constInd, constVals, GRB_EQUAL, 1.0, constName);
                if (error) {
                    printf("ERROR %d GRBaddconstr(%s): %s\n", error, constName, GRBgeterrormsg(env));
                    noErrors = false;
                    break;
                }
            }
        }
        if (noErrors == false) {
            break;
        }
    }

    /*create equation for every value in every row, column and block*/
    for (neighbourType = 0; neighbourType <= 2; neighbourType++) {
        char neighbourTypeName[10];

        if (noErrors == false) {
            break;
        }

        switch (neighbourType) {
            case 0: {
                /* rows */
                sprintf(neighbourTypeName, "%s", "row");
                break;
            }
            case 1: {
                /* columns */
                sprintf(neighbourTypeName, "%s", "column");
                break;
            }
            case 2: {
                /* blocks */
                sprintf(neighbourTypeName, "%s", "block");
                break;
            }
            default: {
                printError(EUnreachableCode);
            }
        }

        for (k = 1; k <= g_gameDim.N; k++) {
            for (i = 0; i < g_gameDim.N; i++) {
                int relvantVarsCount = 0;
                char constName[MAX_CONST_NAME_LEN];

                for (j = 0; j < g_gameDim.N; j++) {
                    PossibleVar *posVar;

                    switch (neighbourType) {
                        case 0: {
                            /* rows */
                            posVar = getPossibleVarFromCoor2Var(coor2Var, createCoordinate(i, j), k);
                            break;
                        }
                        case 1: {
                            /* columns */
                            posVar = getPossibleVarFromCoor2Var(coor2Var, createCoordinate(j, i), k);
                            break;
                        }
                        case 2: {
                            /* blocks */
                            posVar = getPossibleVarFromCoor2Var(coor2Var, coordinateOfTheJCellInTheIBlock(i, j), k);
                            break;
                        }
                        default: {
                            printError(EUnreachableCode);
                            exit(-1);
                        }
                    }

                    if (posVar != NULL) {
                        if (posVar->varIndex >= 0) {
                            constInd[relvantVarsCount] = posVar->varIndex;
                            relvantVarsCount++;
                        }
                    }

                }

                if (relvantVarsCount > 0) {
                    sprintf(constName, "%s #%d with value of %d", neighbourTypeName, i + 1, k);

                    /* add constraint to model*/
                    error = GRBaddconstr(model, relvantVarsCount, constInd, constVals, GRB_EQUAL, 1.0, constName);
                    if (error) {
                        printf("ERROR %d GRBaddconstr(%s): %s\n", error, constName, GRBgeterrormsg(env));
                        noErrors = false;
                        break;
                    }
                }
            }
            if (noErrors == false) {
                break;
            }
        }
        if (noErrors == false) {
            break;
        }
    }

    free(constInd);
    free(constVals);

    return noErrors;
}

Bool optimizeModel() {
    int error;
    int optimstatus;

    /* Optimize model - need to call this before calculation */
    error = GRBoptimize(model);
    if (error) {
        printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
        return false;
    }

    /* Write model to 'mip1.lp' - this is not necessary but very helpful */
    error = GRBwrite(model, GUR_LOG_FILE2);
    if (error) {
        printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
    }

    /* get objective value*/
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
        return false;
    }

    if (optimstatus == GRB_OPTIMAL) { /* solution found */
        return true;
    } else if (optimstatus == GRB_INF_OR_UNBD ||
               optimstatus == GRB_INFEASIBLE ||
               optimstatus == GRB_UNBOUNDED) { /* no solution found */
        /*printf("Model is infeasible or unbounded\n");*/
        return false;
    } else { /* error or calculation stopped */
        printf("Optimization was stopped early, status: %d\n", optimstatus);
        return false;
    }
}

Bool fillModel(PossibleVarSentinel *coor2Var) {

    if (addVarsToModel(coor2Var) == false) {
        return false;
    }

    if (addConstrainsToModel(coor2Var) == false) {
        return false;
    }

    return (optimizeModel());
}

Bool fillBoard(Board board) {
    int error;
    PossibleVarSentinel *coor2Var; /* for the possibleVars[] */
    int i, j, k;
    Bool noErrors;

    coor2Var = createCoor2Var(board, true);
    if (coor2Var == NULL) {
        return false;
    }

    noErrors = fillModel(coor2Var);

    if (noErrors == true) {
        /*get solutions*/
        for (i = 0; i < g_gameDim.N; i++) {
            for (j = 0; j < g_gameDim.N; j++) {
                for (k = 1; k <= g_gameDim.N; k++) {
                    PossibleVar *posVar = getPossibleVarFromCoor2Var(coor2Var, createCoordinate(i, j), k);
                    if (posVar != NULL) {
                        if (posVar->varIndex >= 0) {
                            error = GRBgetdblattrelement(model, GRB_DBL_ATTR_X, posVar->varIndex, &posVar->prob);

                            if (error) {
                                printf("ERROR %d GRBgetdblattrelement(%s): %s\n", error, posVar->name,
                                       GRBgeterrormsg(env));
                                noErrors = false;
                                break;
                            }
                        }

                        if (posVar->prob == 1) {
                            board[i][j] = k;
                        }
                    }
                }
                if (noErrors == false) {
                    break;
                }
            }
            if (noErrors == false) {
                break;
            }
        }
    }

    destroyCoor2Var(coor2Var);

    return noErrors;
}

Bool guessFillBoardAndGuessHint(Board board, Coordinate coor) {
    int error;
    PossibleVarSentinel *coor2Var; /* for the possibleVars[] */
    int k;
    Bool noErrors;

    coor2Var = createCoor2Var(board, false);
    if (coor2Var == NULL) {
        return false;
    }

    noErrors = fillModel(coor2Var);

    if (noErrors == true) {
        for (k = 1; k <= g_gameDim.N; k++) {
            PossibleVar *posVar = getPossibleVarFromCoor2Var(coor2Var, coor, k);
            if (posVar != NULL) {
                if (posVar->varIndex >= 0) {
                    error = GRBgetdblattrelement(model, GRB_DBL_ATTR_X, posVar->varIndex, &posVar->prob);
                    if (error) {
                        printf("ERROR %d GRBgetdblattrelement(): %s\n", error, GRBgeterrormsg(env));
                        noErrors = false;
                        break;
                    }
                }

                if (posVar->prob > 0) {
                    printf("%d -> %f\n", k, posVar->prob);
                }
            }
        }
    }
    destroyCoor2Var(coor2Var);

    return noErrors;
}

Bool guessFillBoard(Board board, double threshold) {
    int error;
    PossibleVarSentinel *coor2Var; /* for the possibleVars[] */
    int i, j, k;
    WeightedValue *weightedValues;
    int overThresholdCount;
    Bool noErrors;

    coor2Var = createCoor2Var(board, false);
    if (coor2Var == NULL) {
        return false;
    }

    noErrors = fillModel(coor2Var);
    if (noErrors == true) {
        weightedValues = (WeightedValue *) smartMalloc(g_gameDim.N * sizeof(WeightedValue));

        for (i = 0; i < g_gameDim.N; i++) {
            for (j = 0; j < g_gameDim.N; j++) {
                overThresholdCount = 0;

                for (k = 1; k <= g_gameDim.N; k++) {
                    PossibleVar *posVar = getPossibleVarFromCoor2Var(coor2Var, createCoordinate(i, j), k);
                    if (posVar != NULL) {
                        if (posVar->varIndex >= 0) {
                            error = GRBgetdblattrelement(model, GRB_DBL_ATTR_X, posVar->varIndex, &posVar->prob);

                            if (error) {
                                printf("ERROR %d GRBgetdblattrelement(): %s\n", error, GRBgeterrormsg(env));
                                noErrors = false;
                                break;
                            }
                        }

                        /* if this prob is as high as threshold
                         * AND
                         * if this value is possible value for the updated board */
                        if (posVar->prob >= threshold && isPossibleValue(board, createCoordinate(i, j), k)) {

                            /* save this value as an option to choose*/
                            weightedValues[overThresholdCount].value = k;
                            weightedValues[overThresholdCount].weight = posVar->prob;

                            overThresholdCount++;
                        }
                    }
                }
                if (noErrors == false) {
                    break;
                }

                /* if at least one value of the highest prob has prob is above threshold */
                if (overThresholdCount > 0) {
                    board[i][j] = getRandomWeightedValue(weightedValues, overThresholdCount);
                }
            }
            if (noErrors == false) {
                break;
            }
        }
        free(weightedValues);
    }

    destroyCoor2Var(coor2Var);

    return noErrors;
}

/* Public Functions */

Bool fillSolutionMatrix(Board board, Board solutionBoard) {
    Bool isSuccess;
    copyBoard(solutionBoard, board);


    isSuccess = (initGurobiEnv() == true) &&
                (fillBoard(solutionBoard) == true) &&
                isBoardComplete(solutionBoard) == true;

    destroyGurobiEnv();
    return isSuccess;
}

/**
 * print the coordinate probabilities that resulted from the LP that are higher then 0
 * @param board
 * @param coordinate
 */
void guessHint(Board board, Coordinate coordinate) {
    Bool isSuccess;
    Board tempBoard = createBoard();
    copyBoard(tempBoard, board);

    copyBoard(tempBoard, board);


    isSuccess = (initGurobiEnv() == true) &&
                (guessFillBoardAndGuessHint(tempBoard, coordinate) == true) &&
                isBoardErroneous(tempBoard) == false;

    destroyBoard(tempBoard, g_gameDim);
    destroyGurobiEnv();

    if (isSuccess == false) {
        printError(EGuessHintFailed);
    }
}

Bool guessBoard(Board board, Board solutionBoard, double threshold) {
    Bool isSuccess;
    copyBoard(solutionBoard, board);


    isSuccess = (initGurobiEnv() == true) &&
                (guessFillBoard(solutionBoard, threshold) == true) &&
                isBoardErroneous(solutionBoard) == false;

    destroyGurobiEnv();
    return isSuccess;
}

