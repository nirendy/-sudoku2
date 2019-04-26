#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include "SPBufferset.h"
#include "file_handler.h"
#include "linked_list.h"


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
            executeCommand(input, gameP);

            saveGameToFile("../data/11", *gameP);

            setGameDim(2, 2);
            *gameP = createGame();
            executeCommand(input, gameP);
            saveGameToFile("../data/22", *gameP);

            setGameDim(3, 3);
            *gameP = createGame();
            executeCommand(input, gameP);
            saveGameToFile("../data/33", *gameP);

            setGameDim(4, 4);
            *gameP = createGame();
            executeCommand(input, gameP);
            saveGameToFile("../data/44", *gameP);

            setGameDim(5, 5);
            *gameP = createGame();
            executeCommand(input, gameP);
            saveGameToFile("../data/55", *gameP);

            setGameDim(2, 3);
            *gameP = createGame();
            executeCommand(input, gameP);
            saveGameToFile("../data/23", *gameP);

            setGameDim(4, 3);
            *gameP = createGame();
            executeCommand(input, gameP);
            saveGameToFile("../data/43", *gameP);

            setGameDim(1, 4);
            *gameP = createGame();
            executeCommand(input, gameP);
            saveGameToFile("../data/14", *gameP);

            setGameDim(2, 5);
            *gameP = createGame();
            executeCommand(input, gameP);
            saveGameToFile("../data/25", *gameP);

            setGameDim(3, 1);
            *gameP = createGame();
            executeCommand(input, gameP);
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
            executeCommand(input, gameP);

            input.command = COMMAND_SAVE;
            sprintf(input.path, "%s", "../data/saved_-1");
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);

            break;
        }
        case -2: {
            /*solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-2");
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);

            break;
        }
        case -3: {
            /*non solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-3");
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);
            break;

        }
        case -4: {
            /*4X4 solvable board*/

            input.command = COMMAND_EDIT;
            sprintf(input.path, "%s", "../data/-4");
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);
            break;
        }
        case -5: {
            /*5X5 solvable board*/

            input.command = COMMAND_EDIT;
            sprintf(input.path, "%s", "../data/-5");
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);

            input.command = COMMAND_NUM_SOLUTIONS;
            executeCommand(input, gameP);

            break;
        }
        case -6: {
            /*non solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-6");
            executeCommand(input, gameP);

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);
            break;
        }
        case -7: {
            /*non solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-7");
            executeCommand(input, gameP);

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);
            break;
        }
        case -8: {
            input.command = COMMAND_EDIT;
            sprintf(input.path, "%s", "../data/-8_hard_5X5_board");
            executeCommand(input, gameP);

            input.command = COMMAND_SAVE;
            sprintf(input.path, "%s", "../data/saved_-8_hard_5X5_board");
            executeCommand(input, gameP);
        }
        case -9: {
            input.command = COMMAND_EDIT;
            input.path[0] = '\0';
            executeCommand(input, gameP);

            input.command = COMMAND_GENERATE;
            input.gen1 = 0;
            input.gen2 = 30;
            executeCommand(input, gameP);

            input.command = COMMAND_GENERATE;
            input.gen1 = 20;
            input.gen2 = 30;
            executeCommand(input, gameP);

            break;
        }
        case -10: {
            /*non solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-10");
            executeCommand(input, gameP);

            input.command = COMMAND_NUM_SOLUTIONS;
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);
/*

            generateInput.command = COMMAND_GENERATE;
            generateInput.gen1 = 0;
            generateInput.gen2 = 81;
            executeCommand(generateInput , gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);
*/
            break;
        }

        case -11: {
            /*non solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-11");
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);
            break;
        }

        case -12: {
            /*non solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-12");
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);
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
    Game *game = NULL;
    g_mode = Init;
    g_markError = true;
    g_curNode = CreateFirstNode();
    SP_BUFF_SET();
    printPrompt(PTitle, 0);

    /*mock(-10, &game);*/

    while (g_mode != Exit) {
        if (askUserForNextTurn(&input)) {
            if (checkLegalInput(input, game)) {
                executeCommand(input, &game);
            }
        }
    }

    return 0;
}

