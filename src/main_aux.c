#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main_aux.h"
#include "solver.h"
#include "parser.h"


void printError(Error err, Command command) {
    if (err == EFunctionFailed && command == COMMAND_INVALID) {
        printf("Unreachable Code Error");
        exit(0);
    }

    switch (err) {
        case EInvalidNumberOfCells:
            printf("Error: invalid number of cells to fill (should be between 0 and %d)\n", n * n * m * m - 1);
            break;
        case ECellIsFixed:
            printf("Error: cell is fixed\n");
            break;
        case EValueIsInvalid:
            printf("Error: value is invalid\n");
            break;
        case EInvalidCommand:
            printf("Error: invalid command\n");
            break;
        case EFunctionFailed:
            /* TODO: fill command*/
            printf("Error: <%d> has failed\n", command);
            break;
    }
}

void printPrompt(Prompt prompt, int num1) {
    switch (prompt) {
        case PEnterFixedAmount:
            printf("Please enter the number of cells to fill [0-%d]:\n", n * n * m * m - 1);
            break;
        case PExit:
            printf("Exiting...\n");
            break;
        case PSuccess:
            printf("Puzzle solved successfully\n");
            break;
        case PHint:
            printf("Hint: set cell to %d\n", num1);
            break;
        case PValidateFailed:
            printf("Validation failed: board is unsolvable\n");
            break;
        case PValidateSuccess:
            printf("Validation passed: board is solvable\n");
            break;

        case PTitle:
            printf("Sudoku Game\n");
            break;
    }
}

Coordinate createCoordinate(int i, int j) {
    Coordinate result;
    result.i = i;
    result.j = j;
    return result;
}

int randLimit(int limit) {
    return rand() % limit;
}

/*return 0 only if finished successfully*/
FinishCode askUserForDimension() {
    return parseDimension();
}

/*return 0 only if finished successfully*/
FinishCode askUserForHintsAmount(int *hintsAmount) {
    FinishCode finishCode;
    do {
        finishCode = parseHintsAmount(hintsAmount);
        if (!(finishCode == FC_SUCCESS || finishCode == FC_INVALID_RECOVERABLE)) {
            return finishCode;
        }
    } while (finishCode == FC_INVALID_RECOVERABLE);

    return FC_SUCCESS;
}

Game *createGame() {
    Game *game = malloc(sizeof(Game));
    game->solved_matrix = (int **) malloc(n * m * sizeof(int *));
    game->user_matrix = (int **) malloc(n * m * sizeof(int *));
    game->fixed_matrix = (Bool **) malloc(n * m * sizeof(Bool *));
    {
        int i;
        for (i = 0; i < n * m; ++i) {
            game->solved_matrix[i] = (int *) malloc(n * m * sizeof(int));
            game->user_matrix[i] = (int *) malloc(n * m * sizeof(int));
            game->fixed_matrix[i] = (Bool *) malloc(n * m * sizeof(Bool));
        }
    }

    return game;
}

void destroyGame(Game *game) {
    {
        int i;
        for (i = 0; i < n * m; ++i) {
            free(game->solved_matrix[i]);
            free(game->user_matrix[i]);
            free(game->fixed_matrix[i]);
        }
    }

    free(game->solved_matrix);
    free(game->user_matrix);
    free(game->fixed_matrix);
    free(game);
}

FinishCode askUserForNextTurn(Mode mode, Input *input) {
    FinishCode finishCode;
    do {
        finishCode = parseCommand(input);
        if (!(finishCode == FC_SUCCESS || finishCode == FC_INVALID_RECOVERABLE)) {
            return finishCode;
        }
    } while (finishCode == FC_INVALID_RECOVERABLE);

    return FC_SUCCESS;
}

void terminate(Game *game, FinishCode finishCode) {
    destroyGame(game);
    printPrompt(PExit, COMMAND_INVALID);

    if (finishCode == FC_UNEXPECTED_ERROR || finishCode == FC_INVALID_TERMINATE) {
        exit(-1);
    }
    exit(0);
}


FinishCode executeCommand(Input input, Mode *mode, Game *game) {
    /*
     * game = createGame();
     */
    /*
     * Keep doing until exit
     * */


    /*
         finishCode = askUserForHintsAmount(&fixedAmount);
        if (finishCode != FC_SUCCESS) {
            terminate(game, finishCode);
        };
        generateGame(game, fixedAmount);
        shouldRestart = false;
        printBoard(game->user_matrix, game->fixed_matrix);
*/
    /*
     * Keep doing until restart
     * */

    /*
     while (!shouldRestart) {
        finishCode = askUserForNextTurn(&input);
        if (finishCode != FC_SUCCESS) {
            terminate(game, finishCode);
        };

    }

     */


    switch (input.command) {
        case COMMAND_SET:
            !isSolved(game) ? setCoordinate(game, input) : printError(EInvalidCommand, COMMAND_INVALID);
            break;
        case COMMAND_HINT:
            !isSolved(game) ? hint(game, input.coordinate) : printError(EInvalidCommand, COMMAND_INVALID);
            break;
        case COMMAND_VALIDATE:
            !isSolved(game) ? validate(game) : printError(EInvalidCommand, COMMAND_INVALID);
            break;
        case COMMAND_EXIT:
            *mode = Exit;
            terminate(game, FC_SUCCESS);
            break;
        case COMMAND_INVALID:
            printf("Unreachable Code Error\n");
            terminate(game, FC_UNEXPECTED_ERROR);
    }

    printBoard(game->user_matrix, game->fixed_matrix);
    /*terminate(game, FC_SUCCESS);*/

    return FC_SUCCESS;
}
