#include "game.h"
#include "solver.h"
#include "parser.h"

Bool isCoordinateFixed(Game *game, Coordinate coordinate) {
    return game->fixed_matrix[coordinate.i][coordinate.j] == true;
}

Bool isValueInNeighbours(Game *game, Coordinate coordinate, int value) {
    int *possibleValues;
    Bool returnValue = true;
    int i, possibleValuesCount;

    possibleValues = (int *) malloc(gameDim.N * sizeof(int));

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

void setCoordinate(Game *game, Input input) {
    if (isCoordinateFixed(game, input.coordinate)) {
        printError(ECellIsFixed, COMMAND_INVALID);
        return;
    }

    game->user_matrix[input.coordinate.i][input.coordinate.j] = 0;
    updateAfterClearErrorMatrix(game, input);
    if (input.value != 0) {
        updateAfterSetErrorMatrix(game, input);
        game->user_matrix[input.coordinate.i][input.coordinate.j] = input.value;
    }

    if (isSolved(game)) {
        printPrompt(PSuccess, 0);
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
