#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include "SPBufferset.h"

void terminate(Game *game, FinishCode finishCode) {
    destroyGame(game);
    printPrompt(PExit, COMMAND_INVALID);

    if (finishCode == FC_UNEXPECTED_ERROR || finishCode == FC_INVALID_TERMINATE) {
        exit(-1);
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    /*
     * init
     * */
    Input input;
    Bool shouldRestart, isGameOver = false;
    Game *game;
    FinishCode finishCode;
    int fixedAmount;
    SP_BUFF_SET();

    /*
     * Set Seed, default to 0
     * */
    if (argc > 1) {
        srand((unsigned int) strtol(*(argv + 1), (char **) NULL, 10));
    } else {
        srand(0);
    }

    game = createGame();
    /*
     * Keep doing until exit
     * */
    while (!isGameOver) {
        finishCode = askUserForHintsAmount(&fixedAmount);
        if (finishCode != FC_SUCCESS) {
            terminate(game, finishCode);
        };
        generateGame(game, fixedAmount);
        shouldRestart = false;
        printBoard(game->user_matrix, game->fixed_matrix);

        /*
         * Keep doing until restart
         * */
        while (!shouldRestart) {
            finishCode = askUserForNextTurn(&input);
            if (finishCode != FC_SUCCESS) {
                terminate(game, finishCode);
            };

            switch (input.command) {
                case COMMAND_SET:
                    !isSolved(game) ? setCoordinate(game, input) : printError(EInvalidCommand, COMMAND_INVALID);
                    break;
                case COMMAND_HINT:
                    !isSolved(game) ? hint(game, input.coordinate) : printError(EInvalidCommand, COMMAND_INVALID);
                    break;
                case COMMAND_VALIDATE:
                    !isSolved(game) ? validate(game) : printError(EInvalidCommand, COMMAND_INVALID);
                    break;
                case COMMAND_RESTART:
                    shouldRestart = true;
                    break;
                case COMMAND_EXIT:
                    shouldRestart = true;
                    isGameOver = true;
                    break;
                case COMMAND_INVALID:
                    printf("Unreachable Code Error\n");
                    terminate(game, FC_UNEXPECTED_ERROR);
            }
        }

    }

    terminate(game, FC_SUCCESS);
    return 0;
}
