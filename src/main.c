#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include "SPBufferset.h"
#include "file_handler.h"
#include "linked_list.h"

int main(int argc, char *argv[]) {
    Input input;
    Game *game;
    mode = Init;
    curNode = CreateFirstNode();
    SP_BUFF_SET();
    printPrompt(PTitle, 0);

    /* MOCK */
    mode = Solve;
//    setGameDim(2,2);
//
    setDimentiosFromFile("../data/1");
    game = createGame();
//    generateGame(game, 0);

    generateGameFromFile("../data/1", game);

//    saveGameToFile("../data/1", game);
    /*
     */
    /* END MOCK*/


    while (mode != Exit) {
        if (askUserForNextTurn(mode, &input)) {
            if (checkLegalInput(input, game)) {
                executeCommand(input, &game);
            }
        }
    }
    return 0;
}

