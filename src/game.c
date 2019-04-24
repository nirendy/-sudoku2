#include "game.h"
#include "solver.h"
#include "parser.h"

Bool isCoordinateFixed(Game *game, Coordinate coordinate) {
    return game->fixed_matrix[coordinate.i][coordinate.j] == true;
}

Bool isCoordinateEmpty(Game *game, Coordinate coordinate) {
    return game->user_matrix[coordinate.i][coordinate.j] == 0;
}

Bool isValueInNeighbours(Game *game, Coordinate coordinate, int value) {
    int *possibleValues;
    Bool returnValue = true;
    int i, possibleValuesCount;

    possibleValues = (int *) malloc(g_gameDim.N * sizeof(int));

    possibleValuesCount = getPossibleValues(game->user_matrix, coordinate, possibleValues);

    /*will return false if the value is found in one of the neighbours*/
    for (i = 0; i < possibleValuesCount; ++i) {
        if (possibleValues[i] == value) {
            returnValue = false;
            break;
        }
    }

    /* did not find - return true*/
    free(possibleValues);
    return returnValue;
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