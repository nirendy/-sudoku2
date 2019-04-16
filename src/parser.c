#include "parser.h"

int isFixed(BoolBoard fixed_matrix, int i, int j) {
    return fixed_matrix[i][j];
}

int getType() {
    return 0;
}

/*return 0 only if finished successfully */
FinishCode parseHintsAmount(int *hintsAmount) {
    printPrompt(PEnterFixedAmount, 0);

    /*
     * read input
     * */
    if (scanf("%d", hintsAmount) != 1) {
        char isEOF;
        if (scanf("%c", &isEOF) != EOF) {
            /* This part is just that the programs works as the example program*/
            printf("Error: not a number\n");
            return FC_EOF;
        }
        return FC_INVALID_TERMINATE;
    }

    /*
     * Validate input
     * */
    if (!(0 <= *hintsAmount && *hintsAmount <= gameDim.cellsCount - 1)) {
        printError(EInvalidNumberOfCells, 0);
        *hintsAmount = -1;
        return FC_INVALID_RECOVERABLE;
    }

    return FC_SUCCESS;
}

void printSepRow(int len) {
    int i = 0;
    for (i = 0; i < len; i++) {
        printf("-");
    }
    printf("\n");
}


void printBoard(Board matrix, BoolBoard fixed_matrix) {
    const char SPACE = ' ', PIPE = '|', ASTERISK = '*', DOT = '.', NEWLINE = '\n';
    int len = 4 * gameDim.N + gameDim.m + 1;
    int i = 0, j = 0, k = 0;
    int indexI = 0;
    int type;

    printSepRow(len);
    for (i = 0; i < gameDim.n; i++) {

        for (j = 0; j < gameDim.m; j++) {

            for (k = 0; k < gameDim.N; k++) {

                if (k % gameDim.n == 0) {
                    printf("%c", PIPE);
                }

                printf("%c", SPACE);
                matrix[indexI][k] != 0 ? printf("%2d", matrix[indexI][k]) : printf("  ");
                type = getType();
                switch (type) {
                    case 0:
                        printf("%c", SPACE);
                        break;
                    case 1:
                        printf("%c", DOT);
                        break;
                    case 2:
                        printf("%c", ASTERISK);
                        break;

                }

            }
            indexI++;
            printf("%c", PIPE);
            printf("%c", NEWLINE);

        }
        printSepRow(len);
    }


}


/* Categorize token to commands */
int ClassifyCommand(char *token, Input *returnedInputP) {
    int numOfVars = -1;

    if (!strcmp(token, "solve")) {
        numOfVars = 1;
        returnedInputP->command = SOLVE;
    }
    if (!strcmp(token, "edit")) {
        numOfVars = 1;
        returnedInputP->command = EDIT;
    }
    if (!strcmp(token, "mark_errors")) {
        numOfVars = 1;
        returnedInputP->command = MARK_ERRORS;
    }
    if (!strcmp(token, "print_board")) {
        numOfVars = 0;
        returnedInputP->command = PRINT_BOARD;
    }
    if (!strcmp(token, "set")) {
        numOfVars = 3;
        returnedInputP->command = SET;
    }
    if (!strcmp(token, "validate")) {
        numOfVars = 0;
        returnedInputP->command = VALIDATE;
    }
    if (!strcmp(token, "guess")) {
        numOfVars = 1;
        returnedInputP->command = GUESS;
    }
    if (!strcmp(token, "generate")) {
        numOfVars = 2;
        returnedInputP->command = GENERATE;
    }
    if (!strcmp(token, "undo")) {
        numOfVars = 0;
        returnedInputP->command = UNDO;
    }
    if (!strcmp(token, "redo")) {
        numOfVars = 0;
        returnedInputP->command = REDO;
    }
    if (!strcmp(token, "save")) {
        numOfVars = 1;
        returnedInputP->command = SAVE;
    }
    if (!strcmp(token, "hint")) {
        numOfVars = 2;
        returnedInputP->command = HINT;
    }
    if (!strcmp(token, "guess_hint")) {
        numOfVars = 2;
        returnedInputP->command = GUESS_HINT;
    }
    if (!strcmp(token, "num_solutions")) {
        numOfVars = 0;
        returnedInputP->command = NUM_SOLUTIONS;
    }
    if (!strcmp(token, "autofill")) {
        numOfVars = 0;
        returnedInputP->command = AUTOFILL;
    }
    if (!strcmp(token, "reset")) {
        numOfVars = 0;
        returnedInputP->command = RESET;
    }
    if (!strcmp(token, "exit")) {
        numOfVars = 0;
        returnedInputP->command = EXIT;
    }

    return numOfVars;
}

void initInput(Input *returnedInput) {
    returnedInput->command      =  COMMAND_INVALID;
    returnedInput->coordinate.i =  INVALID_VALUE;
    returnedInput->coordinate.j =  INVALID_VALUE;
    returnedInput->value        =  INVALID_VALUE;
    returnedInput->gen1         =  INVALID_VALUE;
    returnedInput->gen2         =  INVALID_VALUE;
    returnedInput->threshold    =  INVALID_THRESHOLD;
    returnedInput->path[0] = '\0';
}

void fillPath(Input *returnedInput, char *path) {
    strcpy(returnedInput->path, path);
}

int getNum(char *str){
    int i, n = (int) strlen(str);
    int digit , res = 0;
    for ( i = 0; i < n; i++) {
        digit = str[i] - '0';
        if (!(digit >= 0 && digit <= 9)) { return -1; }
        res = res * 10 + digit;
    }

    return res;
}

float getFloat(char *str) {
    int i, n = (int) strlen(str), numOfDots = 0;
    float val = (float) atof(str);
    if (val == 0.0)
    {
        for (i = 0; i < n; i++) {
            if (str[i] != '0' && str[i] != '.') {
                return INVALID_THRESHOLD;
            }
            if (str[i] == '.') {
                numOfDots++;
            }
        }
    if (numOfDots > 1) { return INVALID_THRESHOLD; }
    }
    return val;
}


FinishCode parseCommand(Input *returnedInput) {


    char str[MAX_STRING_LEN];
    char *token, command[MAX_COMMAND_LEN + 1];
    int numOfVars;
    int index = 0;
    initInput(returnedInput);

    /* Do until a non empty line received */
    do {
        if (fgets(str, MAX_STRING_LEN, stdin) == NULL) {
            return FC_EOF;
        }
        token = strtok(str, " \t\r\n");
    } while (token == NULL);

    numOfVars = ClassifyCommand(token, returnedInput);

    if (numOfVars == -1) {
        printError(EInvalidCommand, COMMAND_INVALID);
        return FC_INVALID_RECOVERABLE;
    }

    strcpy(command, token);
    token = strtok(NULL, " \t\r\n");
    index = 1;

    /*while not all expected parameters has been interpreted*/
    while (token != NULL && index <= numOfVars) {

        switch (index) {
            case 1:
                if (!strcmp(token, "solve") || !strcmp(token, "edit") || !strcmp(token, "save")) {fillPath(returnedInput, token); }

                if (!strcmp(token, "generate")){returnedInput->gen1 = getNum(token);}

                if (!strcmp(token, "guess")){returnedInput->threshold = getFloat(token);}

                if (!strcmp(token, "mark_errors")){returnedInput->value = getNum(token);}

                if (!strcmp(token, "hint") || !strcmp(token, "guess_hint") || !strcmp(token, "set")) {returnedInput->coordinate.i = getNum(token) - 1; }

                break;
            case 2:

                if (!strcmp(token, "generate")){returnedInput->gen2 = getNum(token);}

                if (!strcmp(token, "hint") || !strcmp(token, "guess_hint") || !strcmp(token, "set")) {returnedInput->coordinate.j = getNum(token) - 1; }
                break;
            case 3:
                if (!strcmp(token, "set")) {returnedInput->value = getNum(token); }
                break;

            default:
                printf("Unreachable Code Error");
                return FC_UNEXPECTED_ERROR;
        }

        index++;
        token = strtok(NULL, " \t\r\n");
    }

    if (index != numOfVars && strcmp(command, "edit") != 0) {
        printError(EInvalidNumOfParams, returnedInput->command);
        return FC_INVALID_RECOVERABLE;
    }


    return FC_SUCCESS;
}

