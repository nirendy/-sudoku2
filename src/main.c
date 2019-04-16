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
    FinishCode finishCode;
    SP_BUFF_SET();
    printPrompt(PTitle, 0);
    /*askUserForNextTurn(mode, &input);*/

    /* MOCK*/
    mode = Solve;
    setGameDim(3,3);
    game = createGame();
    generateGame(game, 20);

    /*saveFile("../data/1", game);*/

    printBoard(game->user_matrix, game->fixed_matrix);
    /* END MOCK*/



    while (mode != Exit) {
        finishCode = askUserForNextTurn(mode, &input);
        finishCode = executeCommand(input, &mode, game);
    }

    return 0;
}
