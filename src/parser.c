#include "parser.h"

/*Parser module - responsible for parsing user's command to the program as well as printing the board*/

/*Local error prints related functions*/

/**
 *
 * @return mode as string
 */
char *getModeStr() {
    switch (g_mode) {
        case Edit:
            return "Edit";
        case Solve:
            return "Solve";
        case Init:
            return "Init";
        case Exit:
            printError(EUnreachableCode);
    }
    return "Unreachable Code";
}

/**
 *
 * @param command
 * @return command as string
 */
char *getCommandStr(Command command) {
    switch (command) {
        case COMMAND_SOLVE:
            return "solve";

        case COMMAND_EDIT:
            return "edit";

        case COMMAND_MARK_ERRORS:
            return "mark_errors";

        case COMMAND_PRINT_BOARD:
            return "print_board";

        case COMMAND_SET:
            return "set";

        case COMMAND_VALIDATE:
            return "validate";

        case COMMAND_GUESS:
            return "guess";

        case COMMAND_GENERATE:
            return "generate";

        case COMMAND_UNDO:
            return "undo";

        case COMMAND_REDO:
            return "redo";

        case COMMAND_SAVE:
            return "save";

        case COMMAND_HINT:
            return "hint";

        case COMMAND_GUESS_HINT:
            return "guess_hint";

        case COMMAND_NUM_SOLUTIONS:
            return "num_solutions";

        case COMMAND_AUTOFILL:
            return "autofill";

        case COMMAND_RESET:
            return "reset";

        case COMMAND_EXIT:
            return "exit";

        case COMMAND_INVALID:
            printError(EUnreachableCode);

    }
    printError(EUnreachableCode);
    return "Unreachable Code";
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
            printError(EUnreachableCode);
            return false;
        }
    }
}

void printModeError(Command command) {
    printf("Error: command <%s> is not valid in mode <%s>\n", getCommandStr(command), getModeStr());
    printf("The command available in modes:");
    if (isCommandAllowedInMode(Init, command)) { printf(" <Init>"); }
    if (isCommandAllowedInMode(Edit, command)) { printf(" <Edit>"); }
    if (isCommandAllowedInMode(Solve, command)) { printf(" <Solve>"); }
    printf("\n");
}

void printParamError(Command command, int numOfVars) {
    printf("The command <%s> demands exactly %d parameters\n", getCommandStr(command), numOfVars);
}


/*Local board print related functions*/

/*
 * Get type of char to print
 * 0: space, 1: fixed , 2: erroneous*/
int getType(Game *game, int i, int j) {
    if (game->fixed_matrix[i][j] == 1) { return 1; }
    if (game->error_matrix[i][j] == 1 && (g_markError == 1 || g_mode == Edit)) { return 2; }
    return 0;
}

void printSepRow(int len) {
    int i = 0;
    for (i = 0; i < len; i++) {
        printf("-");
    }
    printf("\n");
}

/*Local command parser related functions*/

/* converting int string to int*/
int getNum(char *str) {
    int i, n = (int) strlen(str);
    int digit, res = 0;
    for (i = 0; i < n; i++) {
        digit = str[i] - '0';
        if (!(digit >= 0 && digit <= 9)) { return INVALID_VALUE; }
        res = res * 10 + digit;
    }

    return res;
}

/* Converting float string to float */
float getFloat(char *str) {
    int i, n = (int) strlen(str), numOfDots = 0;
    float val = (float) atof(str);
    if (val == 0.0) {
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

void fillPath(Input *returnedInput, char *path) {
    strcpy(returnedInput->path, path);
}

void initInput(Input *returnedInput) {
    returnedInput->command = COMMAND_INVALID;
    returnedInput->coordinate.i = INVALID_VALUE;
    returnedInput->coordinate.j = INVALID_VALUE;
    returnedInput->value = INVALID_VALUE;
    returnedInput->gen1 = INVALID_VALUE;
    returnedInput->gen2 = INVALID_VALUE;
    returnedInput->threshold = INVALID_THRESHOLD;
    returnedInput->path[0] = '\0';
}

/**
 * classifies the token into a suitable command by modify input parameter
 * @param token command string
 * @param returnedInputP input parameter to modify
 * @return Num of command params
 */
int ClassifyCommand(char *token, Input *returnedInputP) {


    int numOfCommandParams = -1;

    if (!strcmp(token, "solve")) {
        numOfCommandParams = 1;
        returnedInputP->command = COMMAND_SOLVE;
    }
    if (!strcmp(token, "edit")) {
        numOfCommandParams = 1;
        returnedInputP->command = COMMAND_EDIT;
    }
    if (!strcmp(token, "mark_errors")) {
        numOfCommandParams = 1;
        returnedInputP->command = COMMAND_MARK_ERRORS;
    }
    if (!strcmp(token, "print_board")) {
        numOfCommandParams = 0;
        returnedInputP->command = COMMAND_PRINT_BOARD;
    }
    if (!strcmp(token, "set")) {
        numOfCommandParams = 3;
        returnedInputP->command = COMMAND_SET;
    }
    if (!strcmp(token, "validate")) {
        numOfCommandParams = 0;
        returnedInputP->command = COMMAND_VALIDATE;
    }
    if (!strcmp(token, "guess")) {
        numOfCommandParams = 1;
        returnedInputP->command = COMMAND_GUESS;
    }
    if (!strcmp(token, "generate")) {
        numOfCommandParams = 2;
        returnedInputP->command = COMMAND_GENERATE;
    }
    if (!strcmp(token, "undo")) {
        numOfCommandParams = 0;
        returnedInputP->command = COMMAND_UNDO;
    }
    if (!strcmp(token, "redo")) {
        numOfCommandParams = 0;
        returnedInputP->command = COMMAND_REDO;
    }
    if (!strcmp(token, "save")) {
        numOfCommandParams = 1;
        returnedInputP->command = COMMAND_SAVE;
    }
    if (!strcmp(token, "hint")) {
        numOfCommandParams = 2;
        returnedInputP->command = COMMAND_HINT;
    }
    if (!strcmp(token, "guess_hint")) {
        numOfCommandParams = 2;
        returnedInputP->command = COMMAND_GUESS_HINT;
    }
    if (!strcmp(token, "num_solutions")) {
        numOfCommandParams = 0;
        returnedInputP->command = COMMAND_NUM_SOLUTIONS;
    }
    if (!strcmp(token, "autofill")) {
        numOfCommandParams = 0;
        returnedInputP->command = COMMAND_AUTOFILL;
    }
    if (!strcmp(token, "reset")) {
        numOfCommandParams = 0;
        returnedInputP->command = COMMAND_RESET;
    }
    if (!strcmp(token, "exit")) {
        numOfCommandParams = 0;
        returnedInputP->command = COMMAND_EXIT;
    }

    return numOfCommandParams;
}

/*   public functions   */
/* print the board as specified in the project assignment*/
void printBoard(Game *game) {
    const char SPACE = ' ', PIPE = '|', ASTERISK = '*', DOT = '.', NEWLINE = '\n';
    const int sepRowLength = 4 * g_gameDim.N + g_gameDim.m + 1;
    int i = 0, j = 0, k = 0;
    int indexI = 0;
    int type;

    printSepRow(sepRowLength);
    for (i = 0; i < g_gameDim.n; i++) {

        for (j = 0; j < g_gameDim.m; j++) {

            for (k = 0; k < g_gameDim.N; k++) {

                if (k % g_gameDim.n == 0) {
                    printf("%c", PIPE);
                }

                printf("%c", SPACE);
                game->user_matrix[indexI][k] != 0 ? printf("%2d", game->user_matrix[indexI][k]) : printf("  ");
                type = getType(game, indexI, k);
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
                    default:
                        printError(EUnreachableCode);
                }

            }
            indexI++;
            printf("%c", PIPE);
            printf("%c", NEWLINE);

        }
        printSepRow(sepRowLength);
    }


}

/**
 * parse user input line and modify the given input accordinly
 * @param returnedInput the input to modify
 * @return whether the parse was successful
 */
Bool parseCommand(Input *returnedInput) {

    char str[MAX_STRING_LEN];
    char *token, command[MAX_COMMAND_LEN + 1];
    int numOfCommandParams;
    int index = 0;
    initInput(returnedInput);

    /* Do until a non empty line received */
    do {
        if (fgets(str, MAX_STRING_LEN, stdin) == NULL) {
            printError(EReachUnexpectedEOF);
            exit(-1);
        }
        if (strlen(str) >= MAX_INPUT_STR_LEN) {
            printError(EInputTooLong);
            return false;
        }
        token = strtok(str, " \t\r\n");
    } while (token == NULL);

    numOfCommandParams = ClassifyCommand(token, returnedInput);

    if (numOfCommandParams == -1) {
        printError(EInvalidCommand);
        return false;
    }

    strcpy(command, token);

    if (!isCommandAllowedInMode(g_mode, returnedInput->command)) {
        printModeError(returnedInput->command);
        return false;
    }
    token = strtok(NULL, " \t\r\n");
    index = 1;

    /*while not all expected parameters has been interpreted*/
    while (token != NULL) {

        if (index > numOfCommandParams) { /*too many parameters*/
            printError(ETooManyParams);
            printParamError(returnedInput->command, numOfCommandParams);
            return false;
        }
        switch (index) {
            case 1:
                if (!strcmp(command, "solve") || !strcmp(command, "edit") || !strcmp(command, "save")) {
                    fillPath(returnedInput, token);
                }

                if (!strcmp(command, "generate")) { returnedInput->gen1 = getNum(token); }

                if (!strcmp(command, "guess")) { returnedInput->threshold = getFloat(token); }

                if (!strcmp(command, "mark_errors")) { returnedInput->value = getNum(token); }

                if (!strcmp(command, "hint") || !strcmp(command, "guess_hint") ||
                    !strcmp(command, "set")) {
                    returnedInput->coordinate.j = getNum(token) - 1;
                    if (returnedInput->coordinate.j < 0) {
                        returnedInput->coordinate.j = INVALID_VALUE
                    }
                }

                break;
            case 2:

                if (!strcmp(command, "generate")) { returnedInput->gen2 = getNum(token); }

                if (!strcmp(command, "hint") || !strcmp(command, "guess_hint") ||
                    !strcmp(command, "set")) {
                    returnedInput->coordinate.i = getNum(token) - 1;
                    if (returnedInput->coordinate.i < 0) {
                        returnedInput->coordinate.i = INVALID_VALUE
                    }
                }

                break;
            case 3:
                if (!strcmp(command, "set")) { returnedInput->value = getNum(token); }
                break;
            default:
                printError(EUnreachableCode);
        }

        index++;
        token = strtok(NULL, " \t\r\n");
    }

    index--;
    if (index != numOfCommandParams && strcmp(command, "edit") != 0) {
        printError(ETooFewParams);
        printParamError(returnedInput->command, numOfCommandParams);
        return false;
    }


    return true;
}
