#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "linked_list.h"
#include "SPBufferset.h"

#include "file_handler.h" /* TODO: remove*/

void mock(int mockI, Game **gameP) {
    Input input;

    switch (mockI) {
        case 1: {
            g_mode = Solve;
            setGameDim(1, 1);
            *gameP = createGame();

            input.command = COMMAND_GENERATE;
            input.gen1 = 0;
            input.gen2 = 0;
            executeCommand(gameP, input);

            saveGameToFile("../data/11", *gameP);

            setGameDim(2, 2);
            *gameP = createGame();
            executeCommand(gameP, input);
            saveGameToFile("../data/22", *gameP);

            setGameDim(3, 3);
            *gameP = createGame();
            executeCommand(gameP, input);
            saveGameToFile("../data/33", *gameP);

            setGameDim(4, 4);
            *gameP = createGame();
            executeCommand(gameP, input);
            saveGameToFile("../data/44", *gameP);

            setGameDim(5, 5);
            *gameP = createGame();
            executeCommand(gameP, input);
            saveGameToFile("../data/55", *gameP);

            setGameDim(2, 3);
            *gameP = createGame();
            executeCommand(gameP, input);
            saveGameToFile("../data/23", *gameP);

            setGameDim(4, 3);
            *gameP = createGame();
            executeCommand(gameP, input);
            saveGameToFile("../data/43", *gameP);

            setGameDim(1, 4);
            *gameP = createGame();
            executeCommand(gameP, input);
            saveGameToFile("../data/14", *gameP);

            setGameDim(2, 5);
            *gameP = createGame();
            executeCommand(gameP, input);
            saveGameToFile("../data/25", *gameP);

            setGameDim(3, 1);
            *gameP = createGame();
            executeCommand(gameP, input);
            saveGameToFile("../data/31", *gameP);


            break;
        }
        case -1: {
            g_mode = Solve;
            setGameDim(5, 2);
            *gameP = createGame();

            input.command = COMMAND_GENERATE;
            input.gen1 = 0;
            input.gen2 = 95;
            executeCommand(gameP, input);

            input.command = COMMAND_SAVE;
            sprintf(input.path, "%s", "../data/saved_-1");
            executeCommand(gameP, input);

            input.command = COMMAND_VALIDATE;
            executeCommand(gameP, input);

            break;
        }
        case -2: {
            /*solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-2");
            executeCommand(gameP, input);

            input.command = COMMAND_VALIDATE;
            executeCommand(gameP, input);

            break;
        }
        case -3: {
            /*non solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-3");
            executeCommand(gameP, input);

            input.command = COMMAND_VALIDATE;
            executeCommand(gameP, input);
            break;

        }
        case -4: {
            /*4X4 solvable board*/

            input.command = COMMAND_EDIT;
            sprintf(input.path, "%s", "../data/-4");
            executeCommand(gameP, input);

            input.command = COMMAND_VALIDATE;
            executeCommand(gameP, input);
            break;
        }
        case -5: {
            /*5X5 solvable board*/

            input.command = COMMAND_EDIT;
            sprintf(input.path, "%s", "../data/-5");
            executeCommand(gameP, input);

            input.command = COMMAND_VALIDATE;
            executeCommand(gameP, input);

            input.command = COMMAND_NUM_SOLUTIONS;
            executeCommand(gameP, input);

            break;
        }
        case -6: {
            /*non solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-6");
            executeCommand(gameP, input);

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(gameP, input);

            input.command = COMMAND_VALIDATE;
            executeCommand(gameP, input);
            break;
        }
        case -7: {
            /*non solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-7");
            executeCommand(gameP, input);

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(gameP, input);

            input.command = COMMAND_VALIDATE;
            executeCommand(gameP, input);
            break;
        }
        case -8: {
            input.command = COMMAND_EDIT;
            sprintf(input.path, "%s", "../data/-8_hard_5X5_board");
            executeCommand(gameP, input);

            input.command = COMMAND_SAVE;
            sprintf(input.path, "%s", "../data/saved_-8_hard_5X5_board");
            executeCommand(gameP, input);
        }
        case -9: {
            input.command = COMMAND_EDIT;
            input.path[0] = '\0';
            executeCommand(gameP, input);

            input.command = COMMAND_GENERATE;
            input.gen1 = 0;
            input.gen2 = 30;
            executeCommand(gameP, input);

            input.command = COMMAND_GENERATE;
            input.gen1 = 20;
            input.gen2 = 30;
            executeCommand(gameP, input);

            break;
        }
        case -10: {
            /*non solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-10");
            executeCommand(gameP, input);

            input.command = COMMAND_NUM_SOLUTIONS;
            executeCommand(gameP, input);

            input.command = COMMAND_VALIDATE;
            executeCommand(gameP, input);
/*

            generateInput.command = COMMAND_GENERATE;
            generateInput.gen1 = 0;
            generateInput.gen2 = 81;
            executeCommand(generateInput , gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(gameP, input);
*/
            break;
        }

        case -11: {
            /*non solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-11");
            executeCommand(gameP, input);

            input.command = COMMAND_VALIDATE;
            executeCommand(gameP, input);
            break;
        }

        case -12: {
            /*non solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-12");
            executeCommand(gameP, input);

            input.command = COMMAND_VALIDATE;
            executeCommand(gameP, input);
            break;
        }
        case -13: {
            break;
        }

        default: {
            perror("selected mock does not exist\n");
        }

    }
}

int main() {
    Input input;
    Game *game;

    SP_BUFF_SET()

    /* Init Program*/
    game = NULL;
    g_mode = Init;
    g_markError = true;
    g_curNode = CreateFirstNode();
    printPrompt(PTitle, 0);

    /*mock(-10, &game);*/

    /* Game flow */
    while (g_mode != Exit) {                    /* == while(true) */
        if (askUserForNextTurn(&input)) {
            if (checkLegalInput(game, input)) {
                executeCommand(&game, input);
            }
        }
    }

    return 0;
}

