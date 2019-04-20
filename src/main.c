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
    Mode mode = Init;
    Input input;
    Game *game;
    firstNode = CreateFirstNode();
    curNode = firstNode;

    SP_BUFF_SET();
    printPrompt(PTitle, 0);

    /* MOCK */
    mode = Solve;
//    setGameDim(2,2);
//
    setDimentiosFromFile("../data/1");
    game = createGame();
    generateGame(game, 0);

//    generateGameFromFile("../data/1", game);

//    saveGameToFile("../data/1", game);
    /*
     */
    /* END MOCK*/


    while (mode != Exit) {
        askUserForNextTurn(mode, &input);
        executeCommand(input, &mode, &game);
    }

    return 0;
}
