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
            setGameDim(2, 2);
            *gameP = createGame();
            generateGame(*gameP, 0);

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);

            input.command = COMMAND_PRINT_BOARD;
            executeCommand(input, gameP);
            break;
        }
        case 2: {
            input.command = COMMAND_SOLVE;
            sprintf(input.path, "%s", "../data/1");
            executeCommand(input, gameP);

            input.command = COMMAND_VALIDATE;
            executeCommand(input, gameP);

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

    /*mock(1, &game);*/

    while (g_mode != Exit) {
        if (askUserForNextTurn(&input)) {
            if (checkLegalInput(input, game)) {
                executeCommand(input, &game);
            }
        }
    }
    return 0;
}

