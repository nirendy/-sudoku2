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
            printf("Error: invalid number of cells to fill (should be between 0 and %d)\n", N * N * M * M - 1);
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
            printf("Please enter the number of cells to fill [0-%d]:\n", N * N * M * M - 1);
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
    game->solved_matrix = (int **) malloc(N * M * sizeof(int *));
    game->user_matrix = (int **) malloc(N * M * sizeof(int *));
    game->fixed_matrix = (Bool **) malloc(N * M * sizeof(Bool *));
    {
        int i;
        for (i = 0; i < N * M; ++i) {
            game->solved_matrix[i] = (int *) malloc(N * M * sizeof(int));
            game->user_matrix[i] = (int *) malloc(N * M * sizeof(int));
            game->fixed_matrix[i] = (Bool *) malloc(N * M * sizeof(Bool));
        }
    }

    return game;
}

void destroyGame(Game *game) {
    {
        int i;
        for (i = 0; i < N * M; ++i) {
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

FinishCode askUserForNextTurn(Input *input) {
    FinishCode finishCode;
    do {
        finishCode = parseCommand(input);
        if (!(finishCode == FC_SUCCESS || finishCode == FC_INVALID_RECOVERABLE)) {
            return finishCode;
        }
    } while (finishCode == FC_INVALID_RECOVERABLE);

    return FC_SUCCESS;
}
