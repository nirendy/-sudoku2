#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main_aux.h"
#include "solver.h"
#include "parser.h"
#include "file_handler.h"


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
        default: {
            printf("Unreachable Code Error");
        }
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

void setGameDim(int n, int m) {
    gameDim.n = n;
    gameDim.m = m;
    gameDim.N = n * m;
    gameDim.cellsCount = gameDim.N * gameDim.N;
    gameDim.cellNeighboursCount = 3 * gameDim.N - n - m - 1;
}

Board createBoard() {
    Board board = (int **) malloc(gameDim.N * sizeof(int *));
    int i;

    for (i = 0; i < gameDim.N; ++i) {
        board[i] = (int *) malloc(gameDim.N * sizeof(int));
    }

    return board;
}


void destroyBoard(Board board, GameDim dim) {
    {
        int i;
        for (i = 0; i < dim.N; ++i) {
            free(board[i]);
        }
    }

    free(board);
}

BoolBoard createBoolBoard() {
    BoolBoard board = (Bool **) malloc(gameDim.N * sizeof(Bool *));
    int i;

    for (i = 0; i < gameDim.N; ++i) {
        board[i] = (Bool *) malloc(gameDim.N * sizeof(Bool));
    }

    return board;
}

void destroyBoolBoard(BoolBoard board, GameDim dim) {
    {
        int i;
        for (i = 0; i < dim.N; ++i) {
            free(board[i]);
        }
    }

    free(board);
}


Game *createGame() {
    Game *game = malloc(sizeof(Game));
    game->dim = gameDim;
    game->solved_matrix = createBoard();
    game->user_matrix = createBoard();
    game->fixed_matrix = createBoolBoard();

    return game;
}

Game *createGameFromFile(char *filePath) {
    GameDim oldDimentions = gameDim;
    FinishCode finishCode = setDimentiosFromFile(filePath);

    if (finishCode == FC_INVALID_RECOVERABLE) {
        setGameDim(oldDimentions.n, oldDimentions.m);
        return NULL;
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
    destroyBoard(game->solved_matrix, game->dim);
    destroyBoard(game->user_matrix, game->dim);
    destroyBoolBoard(game->fixed_matrix, game->dim);

    free(game);
}

void copyBoard(Board targetBoard, Board copyFromBoard){
    int i, j;
    for (i = 0; i < gameDim.N; ++i) {
        for (j = 0; j < gameDim.N; ++j) {
            targetBoard[i][j] = copyFromBoard[i][j];
        }
    }

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
            Game *newGame = createGameFromFile(input.path);
            if (newGame != NULL) {
                destroyGame(game);
                *gameP = newGame;
                game = newGame; /*TODO: we should keep it, until we be sure it isn't needed*/
                setMode(mode, Solve);
            }
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
                game = newGame; /*TODO: we should keep it, until we be sure it isn't needed*/
                setMode(mode, Solve);
            }
            break;
        }
        case COMMAND_MARK_ERRORS: {
            printf("Command not implemented yet");
            break;
        }
        case COMMAND_PRINT_BOARD: {
            printBoard(game->user_matrix, game->fixed_matrix);
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
            printf("Command not implemented yet");
            break;
        }
        case COMMAND_REDO: {
            printf("Command not implemented yet");
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
            countPossibleSolutions(game->user_matrix);
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

}
