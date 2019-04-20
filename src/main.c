#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include "SPBufferset.h"
#include "file_handler.h"

int main(int argc, char *argv[]) {
    Mode mode = Init;
    Input input;
    Game *game;
    SP_BUFF_SET();
    printPrompt(PTitle, 0);

    /* MOCK */
    mode = Solve;
    setGameDim(3, 5);
    game = createGame();
    generateGame(game, 20);


    /*
    printBoard(game->user_matrix, game->fixed_matrix);
    printBoard(game->solved_matrix, game->fixed_matrix);
    */

    /*
    saveGameToFile("../data/1", game);
    setDimentiosFromFile("../data/1");
    generateGameFromFile("../data/1", game);
     */
    /* END MOCK*/


    while (mode != Exit) {
        askUserForNextTurn(mode, &input);
        executeCommand(input, &mode, &game);
    }

    return 0;
}
