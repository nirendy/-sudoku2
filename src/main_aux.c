#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main_aux.h"
#include "solver.h"
#include "parser.h"
#include "file_handler.h"
#include "linked_list.h"


void printError(Error err, Command command) {
    if (err == EFunctionFailed && command == COMMAND_INVALID) {
        printf("Unreachable Code Error");
        exit(0);
    }

    switch (err) {
        case EInvalidNumberOfCells:
            printf("Error: invalid number of cells to fill (should be between 0 and %d)\n", gameDim.cellsCount - 1);
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
        case EInvalidCommandInMode: {
            printf("Error: <Command> is not valid in <Mode>\n");
            break;
        }
        case EInvalidNumOfParams: {
            printf("Error: number of parameters don't suit the command\n");
            break;
        }
        case EInvalidFile: {
            printf("Error: text in file is invalid\n");
            break;
        }

        case EInvalidFirstParam: {
            printf("Error: first parameter is invalid\n");
            break;
        }

        case ERedoUnavailable: {
            printf("Error: Redo command is not possible\n");
            break;
        }

        case EUndoUnavailable: {
            printf("Error: Undo command is not possible\n");
            break;
        }

        default: {
            printf("Unreachable Code Error");
        }
    }
}

void printPrompt(Prompt prompt, int num1) {
    switch (prompt) {
        case PEnterFixedAmount: {
            printf("Please enter the number of cells to fill [0-%d]:\n", gameDim.cellsCount - 1);
            break;
        }
        case PExit: {
            printf("Exiting...\n");
            break;
        }
        case PSuccess: {
            printf("Puzzle solved successfully\n");
            break;
        }
        case PHint: {
            printf("Hint: set cell to %d\n", num1);
            break;
        }
        case PValidateFailed: {
            printf("Validation failed: board is unsolvable\n");
            break;
        }
        case PValidateSuccess: {
            printf("Validation passed: board is solvable\n");
            break;
        }
        case PTitle: {
            printf("Sudoku Game\n");
            break;
        }
        case PNextCommand: {
            printf("Please enter the desired command:\n");
            break;
        }

        case PPerformedChanges: {
            printf("The performed changes are:\n");
            break;
        }

        default: {
            printf("Unreachable Code Error");
        }
    }
}

void printChange(int i, int j, int value) {
    printf("The value of the cell<%d,%d> set back to %d\n", i, j, value);
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

void setGameDim(int n, int m) {
    gameDim.n = n;
    gameDim.m = m;
    gameDim.N = n * m;
    gameDim.cellsCount = gameDim.N * gameDim.N;
    gameDim.cellNeighboursCount = 3 * gameDim.N - n - m - 1;
}


Game *createGame() {
    Game *game = malloc(sizeof(Game));
    game->solved_matrix = (int **) malloc(gameDim.N * sizeof(int *));
    game->user_matrix = (int **) malloc(gameDim.N * sizeof(int *));
    game->fixed_matrix = (Bool **) malloc(gameDim.N * sizeof(Bool *));
    game->error_matrix = (Bool **) malloc(gameDim.N * sizeof(Bool *));
    {
        int i;
        for (i = 0; i < gameDim.N; ++i) {
            game->solved_matrix[i] = (int *) malloc(gameDim.N * sizeof(int));
            game->user_matrix[i] = (int *) malloc(gameDim.N * sizeof(int));
            game->fixed_matrix[i] = (Bool *) malloc(gameDim.N * sizeof(Bool));
            game->error_matrix[i] = (Bool *) malloc(gameDim.N * sizeof(Bool));
        }
    }

    return game;
}

Game *createGameFromFile(char *filePath) {
    GameDim oldDimentions = gameDim;
    FinishCode finishCode = setDimentiosFromFile(filePath);

    if (finishCode == FC_INVALID_RECOVERABLE) {
        setGameDim(oldDimentions.n, oldDimentions.m);
    } else if (finishCode == FC_SUCCESS) {
        Game *newGame = createGame();
        finishCode = generateGameFromFile(filePath, newGame);

        if (finishCode == FC_INVALID_RECOVERABLE) {
            destroyGame(newGame);
            setGameDim(oldDimentions.n, oldDimentions.m);
            return NULL;
        } else if (finishCode == FC_SUCCESS) {
            return newGame;
        }
    }

    exit(finishCode);
}

void destroyGame(Game *game) {
    {
        int i;
        for (i = 0; i < gameDim.N; ++i) {
            free(game->solved_matrix[i]);
            free(game->user_matrix[i]);
            free(game->fixed_matrix[i]);
            free(game->error_matrix[i]);
        }
    }

    free(game->solved_matrix);
    free(game->user_matrix);
    free(game->fixed_matrix);
    free(game->error_matrix);
    free(game);
}

Bool isCommandAllowedInMode(Mode mode, Command command) {
    switch (command) {
        case COMMAND_SOLVE: {
            return true;
        }
        case COMMAND_EDIT: {
            return true;
        }
        case COMMAND_MARK_ERRORS: {
            return (mode == Solve);
        }
        case COMMAND_PRINT_BOARD: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_SET: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_VALIDATE: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_GUESS: {
            return (mode == Solve);
        }
        case COMMAND_GENERATE: {
            return (mode == Edit);
        }
        case COMMAND_UNDO: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_REDO: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_SAVE: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_HINT: {
            return (mode == Solve);
        }
        case COMMAND_GUESS_HINT: {
            return (mode == Solve);
        }
        case COMMAND_NUM_SOLUTIONS: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_AUTOFILL: {
            return (mode == Solve);
        }
        case COMMAND_RESET: {
            return (mode == Solve || mode == Edit);
        }
        case COMMAND_EXIT: {
            return true;
        }
        case COMMAND_INVALID:
        default: {
            printf("Unreachable Code Error");
            return false;
        }
    }
}

void terminate(Game *game, FinishCode finishCode) {
    destroyGame(game);
    printPrompt(PExit, COMMAND_INVALID);

    if (finishCode == FC_UNEXPECTED_ERROR || finishCode == FC_INVALID_TERMINATE) {
        exit(-1);
    }
    exit(0);
}

void askUserForNextTurn(Mode mode, Input *input) {
    FinishCode finishCode;
    printPrompt(PNextCommand, 0);
    do {
        finishCode = parseCommand(input);
        if (!(finishCode == FC_SUCCESS || finishCode == FC_INVALID_RECOVERABLE)) {
            terminate(NULL, finishCode);
        }

        if (finishCode == FC_SUCCESS && !isCommandAllowedInMode(mode, input->command)) {
            printError(EInvalidCommandInMode, COMMAND_INVALID);
            finishCode = FC_INVALID_RECOVERABLE;
        }
    } while (finishCode == FC_INVALID_RECOVERABLE);
}

void setMode(Mode *mode, Mode newMode) {
    *mode = newMode;
}


void performUndo(Game *game, struct DataNode *currDataNode) {
    Input input;
    currDataNode = getLastDataNode(currDataNode);
    while (currDataNode->isFirst == false) {
        input = currDataNode->undoInput;
        game->user_matrix[input.coordinate.i][input.coordinate.j] = input.value;
        printChange(input.coordinate.i, input.coordinate.j, input.value);
        currDataNode = currDataNode->prev;
    }
}

void performRedo(Game *game, struct DataNode *currDataNode) {
    Input input;
    currDataNode = getFirstDataNode(currDataNode);
    currDataNode = currDataNode->next;
    while (currDataNode!=NULL) {
        input = currDataNode->redoInput;
        game->user_matrix[input.coordinate.i][input.coordinate.j] = input.value;
        printChange(input.coordinate.i, input.coordinate.j, input.value);
        currDataNode = currDataNode->next;
    }
}

void executeCommand(Input input, Mode *mode, Game **gameP) {
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


    Game *game = *gameP;

    switch (input.command) {
        case COMMAND_SOLVE: {
            setDimentiosFromFile(input.path);
            Game *newGame;
            newGame = createGameFromFile(input.path);
            setMode(mode, Solve);
//            if (newGame != NULL) {
//                /* TODO: this destroy isn't good because it doesn't take into account the old dimentions*/
//                destroyGame(game);
//                *gameP = newGame;
//            }
            break;
        }
        case COMMAND_EDIT: {
            Game *newGame;
            if (strlen(input.path) == 0) {
                setGameDim(3, 3);
                newGame = createGame();
                generateGame(game, 0);
            } else {
                newGame = createGameFromFile(input.path);
            }

            if (newGame != NULL) {
                destroyGame(game);
                *gameP = newGame;
                setMode(mode, Edit);
            }
            break;
        }
        case COMMAND_MARK_ERRORS: {
            if (input.value == 0) { markError = false; }
            else if (input.value == 1) { markError = true; }
            else { printError(EInvalidFirstParam, 0); }

            break;
        }
        case COMMAND_PRINT_BOARD: {
            printBoard(game);
            break;
        }
        case COMMAND_SET: {
            /*!isSolved(game) ? setCoordinate(game, input) : printError(EInvalidCommand, COMMAND_INVALID);*/
            setCoordinate(game, input);
            break;
        }
        case COMMAND_VALIDATE: {
            /*!isSolved(game) ? validate(game) : printError(EInvalidCommand, COMMAND_INVALID);*/
            validate(game);
            break;
        }
        case COMMAND_GUESS: {
            printf("Command not implemented yet");
            break;
        }
        case COMMAND_GENERATE: {
            printf("Command not implemented yet");
            break;
        }
        case COMMAND_UNDO: {
            if (curNode->isFirst) { printError(EUndoUnavailable, 0); }
            else {
                printPrompt(PPerformedChanges, 0);
                performUndo(game, curNode->currDataNode);
                curNode = curNode->prev;
            }
            break;
        }
        case COMMAND_REDO: {
            if (curNode->next == NULL) { printError(ERedoUnavailable, 0); }
            else {
                printPrompt(PPerformedChanges, 0);
                performRedo(game, curNode->currDataNode);
                curNode = curNode->next;
            }
            break;
        }
        case COMMAND_SAVE: {
            saveGameToFile(input.path, game);
            break;
        }
        case COMMAND_HINT: {
            /*!isSolved(game) ? hint(game, input.coordinate) : printError(EInvalidCommand, COMMAND_INVALID);*/
            hint(game, input.coordinate);
            break;
        }
        case COMMAND_GUESS_HINT: {
            printf("Command not implemented yet");
            break;
        }
        case COMMAND_NUM_SOLUTIONS: {
            printf("Command not implemented yet");
            break;
        }
        case COMMAND_AUTOFILL: {
            printf("Command not implemented yet");
            break;
        }
        case COMMAND_RESET: {
            printf("Command not implemented yet");
            break;
        }
        case COMMAND_EXIT: {
            terminate(game, FC_SUCCESS);
            break;
        }
        case COMMAND_INVALID: {
            printf("Unreachable Code Error\n");
            terminate(game, FC_UNEXPECTED_ERROR);
        }
    }
    if (input.command == COMMAND_SOLVE ||
        input.command == COMMAND_EDIT ||
        input.command == COMMAND_SET ||
        input.command == COMMAND_AUTOFILL ||
        input.command == COMMAND_UNDO ||
        input.command == COMMAND_REDO ||
        input.command == COMMAND_GENERATE ||
        input.command == COMMAND_GUESS ||
        input.command == COMMAND_RESET) {
        printBoard(game);
    }


}
