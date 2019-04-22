#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include "SPBufferset.h"
#include "file_handler.h"
#include "linked_list.h"

int main() {
    Input input;
    Game *game;
    g_mode = Init;
    g_curNode = CreateFirstNode();
    SP_BUFF_SET();
    printPrompt(PTitle, 0);

    printf("%s", getenv("PATH"));

    /*To avoid uninitialized game warning*/
    game = createGame();
    destroyGame(game);

    /* MOCK
    g_mode = Solve;
    setGameDim(2, 2);
    generateGame(game, 0);
    saveGameToFile("../data/1", game);
    END MOCK*/


    while (g_mode != Exit) {
        if (askUserForNextTurn(g_mode, &input)) {
            if (checkLegalInput(input, game)) {
                executeCommand(input, &game);
            }
        }
    }
    return 0;
}

