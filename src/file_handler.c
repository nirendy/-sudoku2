#include "file_handler.h"

/*File handling module - responsible for dealing with saving|loading files  */

/* Local functions */

Bool isWhiteSpace(char tav) {
    return (tav == ' ' || tav == '\t' || tav == '\r' || tav == '\n');
}

/* convert num char to int */
int getDigit(char tav) {
    return tav - '0';
}

Bool isDigit(char tav) {
    return (tav - '0' >= 0 && tav - '0' <= 9);
}

Bool isLegalNum(int num) {
    return (num >= 0 && num <= g_gameDim.N);
}

Bool isDot(char tav) {
    return tav == '.';
}

void printFileError(char *string) {
    printError(EInvalidFile);
    printf("reason: %s\n", string);
}

/* Public functions - used for saving and loading games from files */

/**
 * Writes the game filePath in the specifed format
 * @param filePath
 * @param game
 */
void saveGameToFile(char *filePath, Game *game) {
    FILE *file;
    int i, j;

    file = fopen(filePath, "w");
    if (file == NULL) {
        printError(EFileSaveFailure);
        return;
    }
    fprintf(file, "%d %d\n", g_gameDim.m, g_gameDim.n);
    for (i = 0; i < g_gameDim.N; i++) {
        for (j = 0; j < g_gameDim.N; j++) {
            if (j > 0) { fprintf(file, " "); }

            fprintf(file, "%d", game->user_matrix[i][j]);

            if (game->fixed_matrix[i][j] == true || (g_mode == Edit && game->user_matrix[i][j] != 0)) {
                fprintf(file, ".");
            }
        }
        fprintf(file, "\n");
    }
    if (fclose(file) == EOF) {
        printError(EFileCloseFailure);
        exit(-1);
    }

    printPrompt(PFileSavedSuccess, 0);
}

/**
 * set the global g_gameDim by the file given
 * @param filePath
 * @return whether or not the file was read successfully
 */
Bool setDimensionsFromFile(char *filePath) {
    FILE *file;
    int tempN, tempM;
    file = fopen(filePath, "r");
    if (file == NULL) {
        printError(EFileOpenFailure);
        return false;
    }
    if (fscanf(file, "%d %d", &tempM, &tempN) != 2) {
        printError(EFileScanFailure);
        return false;
    }
    setGameDim(tempN, tempM);
    if (fclose(file) == EOF) {
        printError(EFileCloseFailure);
        exit(-1);
    }

    return true;
}

/**
 * load the game from the path
 * @param filePath
 * @param game
 * @return whether or not the game was successfully loaded
 */
Bool generateGameFromFile(char *filePath, Game *game) {
    FILE *file;
    int tempN, tempM;
    int i = 0, j = 0, c = 1, num = 0;
    Bool isFailed = false;
    char tav;

    file = fopen(filePath, "r");
    if (file == NULL) {
        printError(EFileOpenFailure);
        return false;
    }

    if (fscanf(file, "%d %d", &tempM, &tempN) != 2) {
        printError(EFileScanFailure);
        return false;
    }


    for (i = 0; i < g_gameDim.N; i++) {
        for (j = 0; j < g_gameDim.N;) {

            if ((c = fgetc(file)) == EOF) { break; }
            tav = (char) c;

            /*first step - search for the first digit*/
            while (!isDigit(tav)) {

                if (!isWhiteSpace(tav)) {
                    printFileError("invalid text format");
                    isFailed = true;
                    break;
                }
                if ((c = fgetc(file)) == EOF) { break; }
                tav = (char) c;

            }

            if (isFailed == true || c == EOF) { break; } /*break if first step failed*/

            game->fixed_matrix[i][j] = 0;
            game->error_matrix[i][j] = 0;

            /*second step - find the rest of the digits*/
            while (isDigit(tav)) {
                num = num * 10 + getDigit(tav);
                if ((c = fgetc(file)) == EOF) { break; }
                tav = (char) c;
            }

            if (!isLegalNum(num)) {
                printFileError("number not in range exists");
                isFailed = true;
                break;
            }

            game->user_matrix[i][j] = num;


            /*third step - find if the last char is EOF or dot or whitespace*/

            if (c == EOF) {
                i++;
                j++;
                break;
            }

            if (!isWhiteSpace(tav) && !isDot(tav)) {
                printFileError("invalid char exists");
                isFailed = true;
                break;
            }

            if (isDot(tav) && num != 0) {
                game->fixed_matrix[i][j] = 1;
            }

            num = 0;
            j++;
        }

        if (isFailed == true || c == EOF) { break; }

    }

    if (!isFailed && (i != g_gameDim.N || j != g_gameDim.N)) {
        printFileError("invalid text - not enough data");
        isFailed = true;
    }

    if (fclose(file) == EOF) {
        printError(EFileCloseFailure);
        exit(-1);
    }


    return !isFailed;
}
