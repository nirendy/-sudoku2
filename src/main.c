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
    Input setInput;
    Input generateInput;

    switch (mockI) {
        case 1: {
            input.command = COMMAND_EDIT;
            sprintf(input.path, "%s", "../data/1");

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(input, gameP);

            setInput.command = COMMAND_SET;
            setInput.coordinate.i = 0;
            setInput.coordinate.j = 0;
            setInput.value = 0;
            executeCommand(setInput, gameP);

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(input, gameP);

            generateInput.command = COMMAND_GENERATE;
            generateInput.gen1 = 3;
            generateInput.gen2 = 0;
            executeCommand(generateInput , gameP);

            break;
        }
        case -1: {
            g_mode = Solve;
            setGameDim(5, 2);
            *gameP = createGame();
            old_generateGame(*gameP, 95);

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

    /*mock(-7, &game);*/

    while (g_mode != Exit) {
        if (askUserForNextTurn(&input)) {
            if (checkLegalInput(input, game)) {
                executeCommand(input, &game);
            }
        }
    }

    return 0;
}

