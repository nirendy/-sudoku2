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
            old_generateGame(*gameP, 0);
            saveGameToFile("../data/11" , *gameP);

            setGameDim(2, 2);
            *gameP = createGame();
            old_generateGame(*gameP, 0);
            saveGameToFile("../data/22", *gameP);

            setGameDim(3, 3);
            *gameP = createGame();
            old_generateGame(*gameP, 0);
            saveGameToFile("../data/33", *gameP);

            setGameDim(4, 4);
            *gameP = createGame();
            old_generateGame(*gameP, 0);
            saveGameToFile("../data/44", *gameP);

            setGameDim(5, 5);
            *gameP = createGame();
            old_generateGame(*gameP, 0);
            saveGameToFile("../data/55", *gameP);

            setGameDim(2, 3);
            *gameP = createGame();
            old_generateGame(*gameP, 0);
            saveGameToFile("../data/23", *gameP);

            setGameDim(4, 3);
            *gameP = createGame();
            old_generateGame(*gameP, 0);
            saveGameToFile("../data/43", *gameP);

            setGameDim(1, 4);
            *gameP = createGame();
            old_generateGame(*gameP, 0);
            saveGameToFile("../data/14", *gameP);

            setGameDim(2, 5);
            *gameP = createGame();
            old_generateGame(*gameP, 0);
            saveGameToFile("../data/25", *gameP);

            setGameDim(3, 1);
            *gameP = createGame();
            old_generateGame(*gameP, 0);
            saveGameToFile("../data/31", *gameP);


            break;
        }
        case -1: {
            g_mode = Solve;
            setGameDim(5, 2);
            *gameP = createGame();
            old_generateGame(*gameP, 95);

            input.command = COMMAND_SAVE;
            sprintf(input.path, "%s", "../data/-1");
            executeCommand(input, gameP);

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(input, gameP);

            break;
        }
        case -2: {
            /*solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-2");
            executeCommand(input, gameP);

            input.command = COMMAND_PRINT_BOARD;
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

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);
            break;

        }
        case -4: {
            /*4X4 solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-4");
            executeCommand(input, gameP);

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);
            break;
        }
        case -5: {
            /*5X5 solvable board*/

            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/-5");
            executeCommand(input, gameP);

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
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

    /*mock(-1, &game);*/

    while (g_mode != Exit) {
        if (askUserForNextTurn(&input)) {
            if (checkLegalInput(input, game)) {
                executeCommand(input, &game);
            }
        }
    }

    return 0;
}

