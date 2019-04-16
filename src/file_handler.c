//
// Created by nir on 16-Apr-19.
//

#include "file_handler.h"


FinishCode saveFile(char *filePath, Game *game) {
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
}


openFile(char *filePath, Game *game) {
    FILE *file;
    int i, j;
    int tempN, tempM;

    file = fopen(filePath, "r"); /*TODO: handle Error*/
    fscanf(file, "%d %d", &tempM, &tempN);/*TODO: handle Error*/
    setGameDim(tempN, tempM);


    fclose(file); /*TODO: handle Error*/
}
