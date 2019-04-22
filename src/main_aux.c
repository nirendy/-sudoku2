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

        case EInvalidSecondParam: {
            printf("Error: second parameter is invalid\n");
            break;
        }

        case EInvalidThirdParam: {
            printf("Error: third parameter is invalid\n");
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
        case ENullNode: {
            printf("Error: Null node given as a pointer - unreachable code\n");
            break;
        }

        case ENullDataNode: {
            printf("Error: Null Data node given as a pointer - unreachable code\n");
            break;
        }

        case EInsertionInMiddle: {
            printf("Error: Insertion in the middle of the list - unreachable code\n");
            break;
        }

        case EErroneousBoard: {
            printf("Error: Board is erroneous\n");
            break;
        }

        default: {
            printf("Unreachable Code Error\n");
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
        case PNumSolutionsOutput: {
            /* TODO: nicer printed message*/
            printf("%d possible solutions\n", num1);
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
    printf("The value of the cell<%d,%d> set back to %d\n", i + 1, j + 1, value);
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

Bool isBoardErroneous(Game *game){
    int i, j;
    for (i = 0; i < gameDim.N; ++i) {
        for (j = 0; j < gameDim.N; ++j) {
            if(game->error_matrix[i][j] == 1)
                return true;
        }
    }
    return false;
}

Game *createGame() {
    Game *game = malloc(sizeof(Game));
    game->dim = gameDim;
    game->solved_matrix = createBoard();
    game->user_matrix = createBoard();
    game->fixed_matrix = createBoolBoard();
    game->error_matrix = createBoolBoard();

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
    destroyBoolBoard(game->error_matrix, game->dim);

    free(game);
}

void copyBoard(Board targetBoard, Board copyFromBoard) {
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

Bool askUserForNextTurn(Mode mode, Input *input) {

    FinishCode finishCode;
    printPrompt(PNextCommand, 0);

    finishCode = parseCommand(input);
    if (!(finishCode == FC_SUCCESS || finishCode == FC_INVALID_RECOVERABLE)) {
        terminate(NULL, finishCode);
        return false;
    }

    if (finishCode == FC_SUCCESS && !isCommandAllowedInMode(mode, input->command)) {
        printError(EInvalidCommandInMode, COMMAND_INVALID);
        return false;
    }

    if (finishCode == FC_INVALID_RECOVERABLE) { return false; }

    return true;
}

void setMode(Mode *mode, Mode newMode) {
    *mode = newMode;
}

void performUndo(Game *game, DataNode *currDataNode, Bool toPrint) {
    Input input;
    currDataNode = getLastDataNode(currDataNode);
    while (currDataNode->isFirst == false) {
        input = currDataNode->undoInput;
        if (toPrint) { printChange(input.coordinate.i, input.coordinate.j, input.value); }
        setCoordinate(game, input);
        currDataNode = currDataNode->prev;
    }
}

void performRedo(Game *game, DataNode *currDataNode) {
    Input input;
    currDataNode = getFirstDataNode(currDataNode);
    currDataNode = currDataNode->next;
    while (currDataNode != NULL) {
        input = currDataNode->redoInput;
        printChange(input.coordinate.i, input.coordinate.j, input.value);
        setCoordinate(game, input);
        currDataNode = currDataNode->next;
    }
}

void setUndoRedoInputs(Game *game, Input in, Input *redo, Input *undo) {

    redo->coordinate = in.coordinate;
    undo->coordinate = in.coordinate;

    redo->value = in.value;
    undo->value = game->user_matrix[in.coordinate.i][in.coordinate.j];

}

void insertInputsToList(Input *redoInputs, Input *undoInputs, int numOfInputs) {
    int k;

    curNode = insertAfterNode(curNode);
    for (k = 0; k < numOfInputs; k++) {
        curNode->currDataNode = insertAfterDataNode(curNode->currDataNode , redoInputs[k] , undoInputs[k]);
    }

}

void performAutoFill(Game *game) {
    Coordinate *emptyCells = (Coordinate *) malloc(gameDim.cellsCount * sizeof(Coordinate));
    int *possibleValues = (int*) malloc(gameDim.cellsCount * sizeof(int));
    Input *cellToFill = (Input*) malloc(gameDim.cellsCount * sizeof(Input));
    Input *redoInputs = (Input*) malloc(gameDim.cellsCount * sizeof(Input));
    Input *undoInputs = (Input*) malloc(gameDim.cellsCount * sizeof(Input));

    int numOfEmpty = getEmptyCells(game->user_matrix , emptyCells);
    int numOfPossibleValues;
    int numOfCellsToFill = 0;

    int k;
    for (k = 0; k < numOfEmpty; k++) {
        numOfPossibleValues = getPossibleValues(game->user_matrix, emptyCells[k] ,possibleValues);
        if(numOfPossibleValues==1){
            cellToFill[numOfCellsToFill].value = possibleValues[0];
            cellToFill[numOfCellsToFill].coordinate = emptyCells[k];
            numOfCellsToFill++;
        }
    }

    if(numOfCellsToFill>0) {

        for (k = 0; k < numOfCellsToFill; k++) {
            setUndoRedoInputs(game, cellToFill[k], &redoInputs[k], &undoInputs[k]);
            if (k < 0) {
                undoInputs[k].value = redoInputs[k - 1].value;
            }
        }

    insertInputsToList(redoInputs,undoInputs,numOfCellsToFill);
        for (k = 0; k < numOfCellsToFill; k++) {
            setCoordinate(game,cellToFill[k]);
        }

    }
    free(emptyCells);
    free(possibleValues);
    free(cellToFill);
    free(redoInputs);
    free(undoInputs);
}

Bool checkLegalInput(Input input, Game *game) {

    switch (input.command) {
        case COMMAND_SOLVE: {
            return true;
        }
        case COMMAND_EDIT: {
            return true;
        }
        case COMMAND_MARK_ERRORS: {
            /*     parameter range check    */
            if(!(input.value==0 || input.value==1)){
                printError(EInvalidFirstParam,0);
                printf("parameter must be a binary number - 0 or 1\n");
                return false;
            }


            return true;
        }
        case COMMAND_PRINT_BOARD: {
            return true;
        }

        case COMMAND_SET: {
            /*     parameter range check    */

            /*First Parameter Check*/
            if (!(input.coordinate.i >= 0 && input.coordinate.i <= gameDim.N - 1)) {
                printError(EInvalidFirstParam, 0);
                printf("parameter must be an integer number between 1 and %d\n", gameDim.N );
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.coordinate.j >= 0 && input.coordinate.j <= gameDim.N - 1)) {
                printError(EInvalidSecondParam, 0);
                printf("parameter must be an integer number between 1 and %d\n", gameDim.N);
                return false;
            }

            /*Third Parameter Check*/
            if (!(input.value >= 0 && input.value <= gameDim.N)) {
                printError(EInvalidThirdParam, 0);
                printf("parameter must be an integer number between 0 and %d\n", gameDim.N);
                return false;
            }

            /*   is parameter legal in current board state check    */

            return true;
        }
        case COMMAND_VALIDATE: {
            /*   is parameter legal in current board state check    */

            if (isBoardErroneous(game)) {
                printError(EErroneousBoard, 0);
                return false;
            }
            return true;
        }
        case COMMAND_GUESS: {
            /*     parameter range check    */

            if(!(input.threshold>=0 && input.threshold<=1)){
                printError(EInvalidFirstParam,0);
                printf("parameter must be a float number between 0 and 1\n");
                return false;
            }
            return true;
        }
        case COMMAND_GENERATE: {
            /*     parameter range check    */

            /*First Parameter Check*/
            if (!(input.gen1 >= 0 && input.gen1 <= gameDim.cellsCount)) {
                printError(EInvalidFirstParam, 0);
                printf("parameter must be an integer number between 0 and %d\n", gameDim.cellsCount);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.gen2 >= 0 && input.gen2 <= gameDim.cellsCount)) {
                printError(EInvalidSecondParam, 0);
                printf("parameter must be an integer number between 0 and %d\n", gameDim.cellsCount);
                return false;
            }

            /*   is parameter legal in current board state check    */


            return true;
        }
        case COMMAND_UNDO: {
            break;
        }
        case COMMAND_REDO: {
            break;
        }
        case COMMAND_SAVE: {
            return true;
        }
        case COMMAND_HINT: {
            /*     parameter range check    */

            /*First Parameter Check*/
            if (!(input.coordinate.i >= 0 && input.coordinate.i <= gameDim.N - 1)) {
                printError(EInvalidFirstParam, 0);
                printf("parameter must be an integer number between 0 and %d\n", gameDim.N - 1);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.coordinate.j >= 0 && input.coordinate.j <= gameDim.N - 1)) {
                printError(EInvalidSecondParam, 0);
                printf("parameter must be an integer number between 0 and %d\n", gameDim.N - 1);
                return false;
            }

            /*   is parameter legal in current board state check    */


            return true;
        }
        case COMMAND_GUESS_HINT: {
            /*     parameter range check    */

            /*First Parameter Check*/
            if (!(input.coordinate.i >= 0 && input.coordinate.i <= gameDim.N - 1)) {
                printError(EInvalidFirstParam, 0);
                printf("parameter must be an integer number between 0 and %d\n", gameDim.N - 1);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.coordinate.j >= 0 && input.coordinate.j <= gameDim.N - 1)) {
                printError(EInvalidSecondParam, 0);
                printf("parameter must be an integer number between 0 and %d\n", gameDim.N - 1);
                return false;
            }

            /*   is parameter legal in current board state check    */


            return true;
        }
        case COMMAND_NUM_SOLUTIONS: {
            return true;
        }
        case COMMAND_AUTOFILL: {
            /*   is parameter legal in current board state check    */

            if (isBoardErroneous(game)) {
                printError(EErroneousBoard, 0);
                return false;
            }
            return true;
        }
        case COMMAND_RESET: {
            return true;
        }
        case COMMAND_EXIT: {
            return true;
        }
        case COMMAND_INVALID: {
            printf("Unreachable Code Error\n");
            terminate(game, FC_UNEXPECTED_ERROR);
            return false;
        }


    }
    return false;
}

void executeCommand(Input input, Game **gameP ) {

    Game *game = *gameP;
    Mode *modePtr = &mode;

    switch (input.command) {
        case COMMAND_SOLVE: {
            Game *newGame = createGameFromFile(input.path);
            if (newGame != NULL) {
                destroyGame(game);
                *gameP = newGame;
                game = newGame; /*TODO: we should keep it, until we be sure it isn't needed*/
                setMode(modePtr, Solve);
                markError = 0;
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
                markError = 1;
            }

            if (newGame != NULL) {
                destroyGame(game);
                *gameP = newGame;
                game = newGame; /*TODO: we should keep it, until we be sure it isn't needed*/
                setMode(modePtr, Edit);
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

            Input redoInput, undoInput;
            setUndoRedoInputs(game, input, &redoInput, &undoInput);
            if (setCoordinate(game, input)) {
                insertInputsToList(&redoInput, &undoInput, 1);
            }

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
                performUndo(game, curNode->currDataNode, true);
                curNode = curNode->prev;
            }
            break;
        }
        case COMMAND_REDO: {
            if (curNode->next == NULL) { printError(ERedoUnavailable, 0); }
            else {
                printPrompt(PPerformedChanges, 0);
                curNode = curNode->next;
                performRedo(game, curNode->currDataNode);
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
            countPossibleSolutions(game->user_matrix);
            break;
        }
        case COMMAND_AUTOFILL: {
            performAutoFill(game);
            break;
        }
        case COMMAND_RESET: {
            while (!curNode->isFirst) {
                performUndo(game, curNode->currDataNode, false);
                curNode = curNode->prev;
            }
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

