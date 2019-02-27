#include "game.h"
#include "solver.h"
#include "parser.h"

Bool isCoordinateFixed(Game *game, Coordinate coordinate) {
    return game->fixed_matrix[coordinate.i][coordinate.j] == true;
}

Bool isValueInNeighbours(Game *game, Coordinate coordinate, int value) {
    int possibleValues[N * M];
    int i, possibleValuesCount;

    possibleValuesCount = getPossibleValues(game->user_matrix, coordinate, possibleValues);

    /*will return false if the value is found in one of the neighbours*/
    for (i = 0; i < possibleValuesCount; ++i) {
        if (possibleValues[i] == value) {
            return false;
        }
    }

    /* did not find - return true*/
    return true;
}

void setCoordinate(Game *game, Input input) {
    if (isCoordinateFixed(game, input.coordinate)) {
        printError(ECellIsFixed, COMMAND_INVALID);
    } else if (input.value != 0 && isValueInNeighbours(game, input.coordinate, input.value)) {
        /* the intention is not to clean,
         * AND
         * one of the neighbours has the value
         * */
        printError(EValueIsInvalid, COMMAND_INVALID);
    } else {
        game->user_matrix[input.coordinate.i][input.coordinate.j] = input.value;
        printBoard(game->user_matrix, game->fixed_matrix);

        /*checks if game is solved*/
        if (isSolved(game)) {
            printPrompt(PSuccess, 0);
        }
    }
}

void hint(Game *game, Coordinate coordinate) {
    printPrompt(PHint, game->solved_matrix[coordinate.i][coordinate.j]);
}

void validate(Game *game) {
    if (solveBoard(game->user_matrix, game->solved_matrix, true)) {
        printPrompt(PValidateSuccess, 0);
    } else {
        printPrompt(PValidateFailed, 0);
    }
}
