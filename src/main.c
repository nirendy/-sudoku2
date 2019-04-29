#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "linked_list.h"
#include "SPBufferset.h"

/* Main Module */

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

