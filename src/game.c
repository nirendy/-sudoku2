#include "game.h"
#include "solver.h"
#include "parser.h"

Bool isCoordinateFixed(Game *game, Coordinate coordinate) {
    return game->fixed_matrix[coordinate.i][coordinate.j] == true;
}

Bool isCoordinateEmpty(Game *game, Coordinate coordinate) {
    return game->user_matrix[coordinate.i][coordinate.j] == 0;
}

Bool setCoordinate(Game *game, Input input) {
    if (isCoordinateFixed(game, input.coordinate)) {
        printError(ECellIsFixed);
        return 0;
    }

    game->user_matrix[input.coordinate.i][input.coordinate.j] = 0;
    updateAfterClearErrorMatrix(game, input);
    if (input.value != 0) {
        updateAfterSetErrorMatrix(game, input);
        game->user_matrix[input.coordinate.i][input.coordinate.j] = input.value;
    }

    return 1;

}

void hint(Game *game, Input input) {

    Board solutionBoard;
    solutionBoard = createBoard();

    if (fillSolutionMatrix(game->user_matrix, solutionBoard)) {
        printPrompt(PHint, solutionBoard[input.coordinate.i][input.coordinate.i]);
    } else {
        printError(EFUnsolvableBoard);
    }

    destroyBoard(solutionBoard, g_gameDim);

}

void validate(Game *game) {
    Board solutionBoard;
    solutionBoard = createBoard();
    if (fillSolutionMatrix(game->user_matrix, solutionBoard)) {
        printPrompt(PValidateSuccess, 0);
    } else {
        printPrompt(PValidateFailed, 0);
    }

    destroyBoard(solutionBoard, g_gameDim);
}

void clearGame(Game *game) {
    clearBoard(game->user_matrix);
    clearBoolBoard(game->fixed_matrix);
    clearBoolBoard(game->error_matrix);
}
