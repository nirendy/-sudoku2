#include "file_handler.h"
FinishCode saveGameToFile(char *filePath, Game *game) {
    FILE *file;
    int i, j;

    file = fopen(filePath, "w"); /*TODO: handle Error*/
    fprintf(file, "%d %d\n", gameDim.m, gameDim.n); /*TODO: handle Error*/
    for (i = 0; i < gameDim.N; i++) {
        for (j = 0; j < gameDim.N; j++) {
            if (j > 0) {
                fprintf(file, " ");/*TODO: handle Error*/
            }

            fprintf(file, "%d", game->user_matrix[i][j]);/*TODO: handle Error*/

            if (game->fixed_matrix[i][j] == true) {
                fprintf(file, ".");/*TODO: handle Error*/
            }
        }
        fprintf(file, "\n");/*TODO: handle Error*/
    }
    fclose(file);/*TODO: handle Error*/
    return FC_SUCCESS;
}


FinishCode setDimentiosFromFile(char *filePath) {
    FILE *file;
    int tempN, tempM;
    file = fopen(filePath, "r"); /*TODO: handle Error*/
    fscanf(file, "%d %d", &tempM, &tempN);/*TODO: handle Error*/
    setGameDim(tempN, tempM);
    fclose(file); /*TODO: handle Error*/
    return FC_SUCCESS;
}

int isWhiteSpace(char tav) {
    return (tav == ' ' || tav == '\t' || tav == '\r' || tav == '\n');
}

int getDigit(char tav) {
    return tav - '0';
}

int isDigit(char tav) {
    return (tav - '0' >= 0 && tav - '0' <= 9);
}

int isLegalNum(int num) {
    return (num >= 0 && num <= gameDim.N);
}

int isDot(char tav) {
    return tav == '.';
}

void printFileError(char *string) {
    printError(EInvalidFile, 0);
    printf("reason: %s\n", string);
}

FinishCode generateGameFromFile(char *filePath, Game *game) {
    FILE *file;
    int tempN, tempM;
    int i = 0, j=0, c = 1, num = 0;
    Bool isFailed = false;
    char tav;
    file = fopen(filePath, "r"); /*TODO: handle Error*/
    fscanf(file, "%d %d", &tempM, &tempN);/*TODO: handle Error*/

    for (i = 0; i < gameDim.N; i++) {
        for (j = 0; j < gameDim.N;) {
            if ((c = fgetc(file)) == EOF) { break; }
            tav = (char) c;

            if (!isDigit(tav)) {
                if (isWhiteSpace(tav)) {
                    continue;
                } else {
                    printFileError("invalid text");
                    isFailed = true;
                    break;
                }

            }

            num = getDigit(tav);
            if (isLegalNum(num)) {
                game->user_matrix[i][j] = num;
            } else {
                printFileError("number not in range exists");
                isFailed = true;
                break;
            }
            game->fixed_matrix[i][j] = 0;
            j++;

            if ((c = fgetc(file)) == EOF) { break; }
            tav = (char) c;

            if (isDigit(tav)) {
                num = num * 10 + getDigit(tav);
                if (isLegalNum(num)) {
                    game->user_matrix[i][j] = num;
                } else {
                    printFileError("number not in range exists");
                    isFailed = true;
                    break;
                }

            } else if (isDot(tav)) {
                game->fixed_matrix[i][j] = 1;
                continue;
            } else if (isWhiteSpace(tav)) { continue; }
            else {
                printFileError("invalid char exists");
                isFailed = true;
                break;
            }
            if ((c = fgetc(file)) == EOF) { break; }
            tav = (char) c;

            if (isDot(tav)) { game->fixed_matrix[i][j] = 1; }
            else {
                if (isDigit(tav)) {
                    printFileError("number not in range exists");
                    isFailed = true;
                    break;
                } else if (isWhiteSpace(tav)) {
                    continue;
                } else {
                    printFileError("invalid text");
                    isFailed = true;
                    break;
                }
            }
        }

        if (isFailed == true || c == EOF) { break; }

    }

    fclose(file); /*TODO: handle Error*/
    if (i!=gameDim.N || j!=gameDim.N){
        if(!isFailed){
            printFileError("invalid text");
            isFailed = true;
        }
    }
    return (isFailed)? FC_INVALID_RECOVERABLE : FC_SUCCESS;
}
