#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include "SPBufferset.h"

int main(int argc, char *argv[]) {
    /*
     * init
     * */
    Input input;
    Bool shouldRestart, isGameOver = false;
    Game *game;
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
        fixedAmount = askUserForHintsAmount();
        generateGame(game, fixedAmount);
        shouldRestart = false;
        printBoard(game->user_matrix, game->fixed_matrix , 3 , 3);

        /*
         * Keep doing until restart
         * */
        while (!shouldRestart) {
            input = askUserForNextTurn();

            switch (input.command) {
                case SET:
                    !isSolved(game) ? setCoordinate(game, input) : printError(EInvalidCommand, INVALID);
                    break;
                case HINT:
                    !isSolved(game) ? hint(game, input.coordinate) : printError(EInvalidCommand, INVALID);
                    break;
                case VALIDATE:
                    !isSolved(game) ? validate(game) : printError(EInvalidCommand, INVALID);
                    break;
//                case RESTART:
//                    shouldRestart = true;
//                    break;
                case EXIT:
                    shouldRestart = true;
                    isGameOver = true;
                    printPrompt(PExit, INVALID);
                    break;
                case INVALID:
                    printf("Unreachable Code Error");
                    exit(0);
            }
        }

    }
    destroyGame(game);
    exit(1);
}
