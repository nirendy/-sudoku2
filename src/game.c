#include "game.h"
#include "solver.h"
#include "parser.h"
#include "linked_list.h"
#include "file_handler.h"
#include "lp_solver.h"


Bool isCoordinateFixed(Game *game, Coordinate coordinate) {
    return game->fixed_matrix[coordinate.i][coordinate.j] == true;
}

Bool isCoordinateEmpty(Game *game, Coordinate coordinate) {
    return game->user_matrix[coordinate.i][coordinate.j] == 0;
}

void printChange(int i, int j, int value) {
    printf("The value of the cell <%d,%d> set back to %d\n", i + 1, j + 1, value);
}

void setCoordinate(Game *game, Input input) {
    game->user_matrix[input.coordinate.i][input.coordinate.j] = 0;
    updateAfterClearErrorMatrix(game, input);
    if (input.value != 0) {
        updateAfterSetErrorMatrix(game, input);
        game->user_matrix[input.coordinate.i][input.coordinate.j] = input.value;
    }
}

void updateRedoUndoInputsAfterSingleSet(Game *game, Input in, Input *redo, Input *undo) {

    redo->coordinate = in.coordinate;
    undo->coordinate = in.coordinate;

    redo->value = in.value;
    undo->value = game->user_matrix[in.coordinate.i][in.coordinate.j]; /*only good for a single set*/

}

void insertInputsToList(Input *redoInputs, Input *undoInputs, int numOfInputs) {
    int k;

    g_curNode = insertAfterNode(g_curNode);
    for (k = 0; k < numOfInputs; k++) {
        g_curNode->currDataNode = insertAfterDataNode(g_curNode->currDataNode, redoInputs[k], undoInputs[k]);
    }

}

void chooseRandCords(Coordinate *source, int sourceSize, Coordinate *target, int targetSize) {
    int i, r, limit;
    int *randIndexes = (int *) smartMalloc(sourceSize * sizeof(int));

    for (i = 0; i < sourceSize; i++) { randIndexes[i] = i; }
    limit = sourceSize;
    for (i = 0; i < targetSize; i++) {
        r = randLimit(limit);
        target[i] = source[randIndexes[r]];
        randIndexes[r] = randIndexes[limit - 1];
        limit--;
    }

    free(randIndexes);
}

int diffToRedoUndoLists(Board original, Board final, Input *redoList, Input *undoList) {

    int i, j, listIndex = 0;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            if (original[i][j] != final[i][j]) {
                redoList[listIndex].coordinate.i = i;
                redoList[listIndex].coordinate.j = j;
                undoList[listIndex].coordinate.i = i;
                undoList[listIndex].coordinate.j = j;
                redoList[listIndex].value = final[i][j];
                undoList[listIndex].value = original[i][j];
                listIndex++;
            }
        }
    }

    return listIndex;
}

int numOfDiffs(Board original, Board final) {
    int i, j, diffs = 0;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            if (original[i][j] != final[i][j]) {
                diffs++;
            }
        }
    }
    return diffs;
}

void performSetsFromRedoList(Game *game, Input *sets, int len) {
    int k;
    for (k = 0; k < len; k++) {
        setCoordinate(game, sets[k]);
    }
}

void updateHistoryList(Game *game, Board final) {

    int numOfSets;
    Input *redoInputs;
    Input *undoInputs;

    Board original = game->user_matrix;
    numOfSets = numOfDiffs(original, final);
    if (numOfSets == 0) {
        destroyBoard(final, g_gameDim);
        return;
    }
    redoInputs = (Input *) smartMalloc(numOfSets * sizeof(Input));
    undoInputs = (Input *) smartMalloc(numOfSets * sizeof(Input));

    diffToRedoUndoLists(original, final, redoInputs, undoInputs);
    insertInputsToList(redoInputs, undoInputs, numOfSets);
    performSetsFromRedoList(game, redoInputs, numOfSets); /*update the user board*/

    destroyBoard(final, g_gameDim);
    free(redoInputs);
    free(undoInputs);
}

Bool fillXRandomCells(Board board, Coordinate *cellsToFill, int numToFill) {

    int k, numOfPossibleValues;
    int *possibleValues = (int *) smartMalloc(g_gameDim.N * sizeof(int *));

    for (k = 0; k < numToFill; k++) {
        numOfPossibleValues = getPossibleValues(board, cellsToFill[k], possibleValues);
        if (numOfPossibleValues == 0) { return false; }
        board[cellsToFill[k].i][cellsToFill[k].j] = possibleValues[randLimit(numOfPossibleValues)];
    }
    return true;
}

void clearRandomCells(Board board, Coordinate *cellsToClear, int numToClear) {

    int k;
    for (k = 0; k < numToClear; k++) {
        board[cellsToClear[k].i][cellsToClear[k].j] = 0;
    }

}

int chooseCellsToFill(Board board, Coordinate *cellsToFill, int sizeToFill) {

    Coordinate *emptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    int numOfEmpty = getEmptyCells(board, emptyCells);
    chooseRandCords(emptyCells, numOfEmpty, cellsToFill, sizeToFill);
    free(emptyCells);
    return numOfEmpty;
}

void chooseCellsToClear(Board board, Coordinate *cellsToClear, int numToClear) {

    Coordinate *filledCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    getFilledCells(board, filledCells); /* TODO: before submitting, use the returned value*/
    chooseRandCords(filledCells, g_gameDim.cellsCount, cellsToClear, numToClear);

    free(filledCells);
}

void fillObviousValues(Board board) {

    Coordinate *emptyCells = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
    int *possibleValues = (int *) smartMalloc(g_gameDim.cellsCount * sizeof(int));

    int numOfEmpty = getEmptyCells(board, emptyCells);
    int numOfPossibleValues;
    int numOfCellsToFill = 0;

    int k;
    for (k = 0; k < numOfEmpty; k++) {
        numOfPossibleValues = getPossibleValues(board, emptyCells[k], possibleValues);
        if (numOfPossibleValues == 1) {
            board[emptyCells[k].i][emptyCells[k].j] = possibleValues[0];
            numOfCellsToFill++;
        }
    }

    free(emptyCells);
    free(possibleValues);
}

Bool isGameErroneous(Game *game) {
    int i, j;
    for (i = 0; i < g_gameDim.N; ++i) {
        for (j = 0; j < g_gameDim.N; ++j) {
            if (game->error_matrix[i][j] == 1) {
                return true;
            }
        }
    }
    return false;
}

Bool isSolvableBoard(Board board) {
    Bool isValid;
    Board solutionBoard = createBoard();
    isValid = fillSolutionMatrix(board, solutionBoard);
    destroyBoard(solutionBoard, g_gameDim);
    return isValid;
}

void clearGame(Game *game) {
    clearBoard(game->user_matrix);
    clearBoolBoard(game->fixed_matrix);
    clearBoolBoard(game->error_matrix);
}

Bool askUserForNextTurn(Input *input) {

    FinishCode finishCode;
    printPrompt(PNextCommand, 0);
    finishCode = parseCommand(input);
    if (!(finishCode == FC_SUCCESS || finishCode == FC_INVALID_RECOVERABLE)) {
        terminateProgram(NULL, finishCode);
        return false;
    }

    if (finishCode == FC_INVALID_RECOVERABLE) { return false; }

    return true;
}

Bool checkLegalInput(Game *game, Input input) {
    /*generate_command vars*/
    Coordinate *tempCorArray;
    int numOfEmptyCells;

    switch (input.command) {
        case COMMAND_SOLVE: {
            return true;
        }
        case COMMAND_EDIT: {
            return true;
        }
        case COMMAND_MARK_ERRORS: {
            /*     parameter range check    */
            if (!(input.value == 0 || input.value == 1)) {
                printError(EInvalidFirstParam);
                printf("parameter must be a binary number - 0 or -1\n");
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
            if (!(input.coordinate.i >= 0 && input.coordinate.i <= g_gameDim.N - 1)) {
                printError(EInvalidFirstParam);
                printf("parameter must be an integer number between 1 and %d\n", g_gameDim.N);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.coordinate.j >= 0 && input.coordinate.j <= g_gameDim.N - 1)) {
                printError(EInvalidSecondParam);
                printf("parameter must be an integer number between 1 and %d\n", g_gameDim.N);
                return false;
            }

            /*Third Parameter Check*/
            if (!(input.value >= 0 && input.value <= g_gameDim.N)) {
                printError(EInvalidThirdParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.N);
                return false;
            }

            /*   is parameter legal in current board state check    */

            if (isCoordinateFixed(game, input.coordinate)) {
                printError(ECellIsFixed);
                return false;
            }


            return true;
        }
        case COMMAND_VALIDATE: {
            /*   is parameter legal in current board state check    */

            if (isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }
            return true;
        }
        case COMMAND_GUESS: {
            /*     parameter range check    */

            if (!(input.threshold >= 0 && input.threshold <= 1)) {
                printError(EInvalidFirstParam);
                printf("parameter must be a float number between 0 and 1\n");
                return false;
            }

            /*   is parameter legal in current board state check    */

            if (isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }
            return true;
        }
        case COMMAND_GENERATE: {
            /*     parameter range check    */

            /*First Parameter Check*/
            if (!(input.gen1 >= 0 && input.gen1 <= g_gameDim.cellsCount)) {
                printError(EInvalidFirstParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.cellsCount);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.gen2 >= 0 && input.gen2 <= g_gameDim.cellsCount)) {
                printError(EInvalidSecondParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.cellsCount);
                return false;
            }

            /*   is parameter legal in current board state check    */


            tempCorArray = (Coordinate *) smartMalloc(g_gameDim.cellsCount * sizeof(Coordinate));
            numOfEmptyCells = getEmptyCells(game->user_matrix, tempCorArray);
            free(tempCorArray);

            if (input.gen1 > numOfEmptyCells) {
                printError(EInvalidFirstParam);
                printf("parameter must be smaller or equal than the number of empty cells\n"); /* TODO: show range */
                return false;
            }

            return true;
        }
        case COMMAND_UNDO: {
            /*   is parameter legal in current board state check    */

            if (g_curNode->isFirst) {
                printError(EUndoUnavailable);
                return false;
            }
            return true;
        }

        case COMMAND_REDO: {
            /*   is parameter legal in current board state check    */

            if (g_curNode->next == NULL) {
                printError(ERedoUnavailable);
                return false;
            }
            return true;
        }
        case COMMAND_SAVE: {
            /*   is parameter legal in current board state check    */

            if (g_mode == Edit && isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }


            return true;
        }
        case COMMAND_HINT: {
            /*     parameter range check    */

            /*First Parameter Check*/
            if (!(input.coordinate.i >= 0 && input.coordinate.i <= g_gameDim.N - 1)) {
                printError(EInvalidFirstParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.N - 1);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.coordinate.j >= 0 && input.coordinate.j <= g_gameDim.N - 1)) {
                printError(EInvalidSecondParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.N - 1);
                return false;
            }

            /*   is parameter legal in current board state check    */
            if (isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }

            if (isCoordinateFixed(game, input.coordinate)) {
                printError(ECellIsFixed);
                return false;
            }

            if (!isCoordinateEmpty(game, input.coordinate)) {
                printError(ECellIsNotEmpty);
                return false;
            }

            return true;
        }
        case COMMAND_GUESS_HINT: {
            /*     parameter range check    */

            /*First Parameter Check*/
            if (!(input.coordinate.i >= 0 && input.coordinate.i <= g_gameDim.N - 1)) {
                printError(EInvalidFirstParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.N - 1);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.coordinate.j >= 0 && input.coordinate.j <= g_gameDim.N - 1)) {
                printError(EInvalidSecondParam);
                printf("parameter must be an integer number between 0 and %d\n", g_gameDim.N - 1);
                return false;
            }

            /*   is parameter legal in current board state check    */
            if (isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }

            if (isCoordinateFixed(game, input.coordinate)) {
                printError(ECellIsFixed);
                return false;
            }

            if (!isCoordinateEmpty(game, input.coordinate)) {
                printError(ECellIsNotEmpty);
                return false;
            }

            return true;
        }
        case COMMAND_NUM_SOLUTIONS: {
            /*   is parameter legal in current board state check    */

            if (isGameErroneous(game)) {
                printError(EErroneousBoard);
                return false;
            }
            return true;
        }
        case COMMAND_AUTOFILL: {
            /*   is parameter legal in current board state check    */

            if (isGameErroneous(game)) {
                printError(EErroneousBoard);
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
            terminateProgram(game, FC_UNEXPECTED_ERROR);
            return false;
        }


    }
    return false;
}


/* Perform Commands */

Bool performSolve(Game **gameP, Input input) {
    Game *game = *gameP; /* alias */

    Game *newGame = createGameFromFile(input.path);
    if (newGame != NULL) {
        if (game != NULL) {          /*skip if first game*/
            destroyGame(game);
        }
        *gameP = newGame;
        game = newGame;
        setMode(Solve);
        updateWholeErrorMatrix(game);
        g_curNode = getFirstNode(g_curNode);
        clearListFromNode(g_curNode->next);
        g_curNode->next = NULL;
        return true;
    }
    return false;
}

Bool performEdit(Game **gameP, Input input) {
    Game *game = *gameP; /* alias */

    Game *newGame;
    if (strlen(input.path) == 0) {
        setGameDim(3, 3);
        newGame = createGame();
        clearGame(newGame);
    } else {
        newGame = createGameFromFile(input.path);
    }

    if (newGame != NULL) {
        if (game != NULL) {         /*skip if first game*/
            destroyGame(game);
        }
        *gameP = newGame;
        game = newGame;
        setMode(Edit);
        clearBoolBoard(game->fixed_matrix);
        updateWholeErrorMatrix(game);
        g_curNode = getFirstNode(g_curNode);
        clearListFromNode(g_curNode->next);
        g_curNode->next = NULL;
        return true;
    }
    return false;
}

void performMarkErrors(Input input) {
    if (input.value == 0) { g_markError = false; }
    else if (input.value == 1) { g_markError = true; }
    else { printf("Unreachable Code"); }
}

void performSet(Game *game, Input input) {
    Input redoInput, undoInput;
    updateRedoUndoInputsAfterSingleSet(game, input, &redoInput, &undoInput);
    insertInputsToList(&redoInput, &undoInput, 1);
    setCoordinate(game, input);
    if (g_mode == Solve && isFullUserBoard(game)) {
        if (!isGameErroneous(game)) {
            printPrompt(PSuccess, 0);
            g_mode = Init;
        } else {
            printPrompt(PWrongSolution, 0);
        }
    }
}

void performValidate(Game *game) {
    if (isSolvableBoard(game->user_matrix)) {
        printPrompt(PValidateSuccess, 0);
    } else {
        printPrompt(PValidateFailed, 0);
    }
}

Bool performGuess(Game *game, Input input) {

    Board solutionBoard = createBoard();
    if (!guessBoard(game->user_matrix, solutionBoard, input.threshold)) {
        destroyBoard(solutionBoard, g_gameDim);
        return false;
    }

    clearListFromNode(g_curNode->next);
    g_curNode->next = NULL;
    updateHistoryList(game, solutionBoard);  /*destroys newBoard*/
    return true;
}

Bool performGenerate(Game *game, Input input) {

    /*variables deceleration*/
    Bool success;
    const int NUM_OF_ITERATIONS = 1000;
    int k, numToFill, numToClear;

    Board newBoard, solutionBoard;
    Coordinate *cellsToFill, *cellsToClear;

    /*variables deceleration*/
    numToFill = input.gen1;
    numToClear = g_gameDim.cellsCount - input.gen2;

    /*step 1 - fill the board */

    /*choose cells to fill*/
    cellsToFill = (Coordinate *) smartMalloc(numToFill * sizeof(Coordinate));

    /*try 1000 times to fill and solve*/
    success = false;
    newBoard = createBoard();
    solutionBoard = createBoard();

    for (k = 0; k < NUM_OF_ITERATIONS; k++) {
        chooseCellsToFill(game->user_matrix, cellsToFill, numToFill);
        copyBoard(newBoard, game->user_matrix);
        if (!fillXRandomCells(newBoard, cellsToFill, numToFill)) { continue; }
        if (!fillSolutionMatrix(newBoard, solutionBoard)) { continue; }

        /*successful solution*/
        copyBoard(newBoard, solutionBoard);
        success = true;
        break;
    }
    destroyBoard(solutionBoard, g_gameDim);
    free(cellsToFill);


    if (!success) {
        printError(EGenerationFailed);
        destroyBoard(newBoard, g_gameDim);
        return success;
    }

    /*step 2 - clear cells from the board */
    if (numToClear > 0) {
        cellsToClear = (Coordinate *) smartMalloc(numToClear * sizeof(Coordinate));
        chooseCellsToClear(newBoard, cellsToClear, numToClear);
        clearRandomCells(newBoard, cellsToClear, numToClear);
        free(cellsToClear);
    }

    /*step 3 - perform changes and update the redo/undo list */
    clearListFromNode(g_curNode->next);
    g_curNode->next = NULL;
    updateHistoryList(game, newBoard); /*destroys newBoard*/
    return true;

}

void performUndo(Game *game, DataNode *currDataNode, Bool toPrint) {
    Input input;

    if (toPrint) {
        printPrompt(PPerformedChanges, 0);
    }

    currDataNode = getLastDataNode(currDataNode);
    while (currDataNode->isFirst == false) {
        input = currDataNode->undoInput;
        if (toPrint) { printChange(input.coordinate.i, input.coordinate.j, input.value); }
        setCoordinate(game, input);
        currDataNode = currDataNode->prev;
    }
    g_curNode = g_curNode->prev;
}

void performRedo(Game *game, DataNode *currDataNode) {
    Input input;

    printPrompt(PPerformedChanges, 0);
    g_curNode = g_curNode->next;

    currDataNode = getFirstDataNode(currDataNode)->next;

    while (currDataNode != NULL) {
        input = currDataNode->redoInput;
        printChange(input.coordinate.i, input.coordinate.j, input.value);
        setCoordinate(game, input);
        currDataNode = currDataNode->next;
    }
}

void performSave(Game *game, Input input) {
    Board solutionBoard = createBoard();

    if (g_mode == Edit && !fillSolutionMatrix(game->user_matrix, solutionBoard)) {
        printError(EFUnsolvableBoard);
    } else { saveGameToFile(input.path, game); }
    destroyBoard(solutionBoard, g_gameDim);
}

void performHint(Game *game, Input input) {

    Board solutionBoard;
    solutionBoard = createBoard();

    if (fillSolutionMatrix(game->user_matrix, solutionBoard)) {
        printPrompt(PHint, solutionBoard[input.coordinate.i][input.coordinate.i]);
    } else {
        printError(EFUnsolvableBoard);
    }

    destroyBoard(solutionBoard, g_gameDim);

}

void performGuessHint(Game *game, Input input) {
    guessHint(game->user_matrix, input.coordinate);
}

void performNumSolutions(Game *game) {
    countPossibleSolutions(game->user_matrix);
}

void performAutoFill(Game *game) {

    Board newBoard = createBoard();
    copyBoard(newBoard, game->user_matrix);
    fillObviousValues(newBoard);
    updateHistoryList(game, newBoard); /*destroys newBoard*/
}

void performReset(Game *game) {
    while (!g_curNode->isFirst) {
        performUndo(game, g_curNode->currDataNode, false);
    }
}


void executeCommand(Game **gameP, Input input) {
    Bool success = false; /* successfully changed board */


    if (input.command == COMMAND_SET ||
        input.command == COMMAND_AUTOFILL) {
        /*
     * This commands cannot fail in this stage so its safe to clear the redo list
     * guess and generate command can fail so clearing the redo list is done after we get success
     * */

        clearListFromNode(g_curNode->next);
        g_curNode->next = NULL; /* TODO: replace to functions*/
    }

    switch (input.command) {
        case COMMAND_SOLVE: {
            success = performSolve(gameP, input);
            break;
        }
        case COMMAND_EDIT: {
            success = performEdit(gameP, input);
            break;
        }
        case COMMAND_MARK_ERRORS: {
            performMarkErrors(input);
            break;
        }
        case COMMAND_PRINT_BOARD: {
            printBoard(*gameP);
            break;
        }
        case COMMAND_SET: {
            performSet(*gameP, input);
            success = true; /*fail condition checked in isLegalMove*/
            break;
        }
        case COMMAND_VALIDATE: {
            performValidate(*gameP);
            break;
        }
        case COMMAND_GUESS: {
            success = performGuess(*gameP, input);
            break;
        }
        case COMMAND_GENERATE: {
            success = performGenerate(*gameP, input);
            break;
        }
        case COMMAND_UNDO: {
            performUndo(*gameP, g_curNode->currDataNode, true);
            success = true; /* fail condition checked in isLegalMove */
            break;
        }
        case COMMAND_REDO: {
            performRedo(*gameP, g_curNode->currDataNode);
            success = true; /* fail condition checked in isLegalMove */
            break;
        }
        case COMMAND_SAVE: {
            performSave(*gameP, input);
            break;
        }
        case COMMAND_HINT: {
            performHint(*gameP, input);
            break;
        }
        case COMMAND_GUESS_HINT: {
            performGuessHint(*gameP, input);
            break;
        }
        case COMMAND_NUM_SOLUTIONS: {
            performNumSolutions(*gameP);
            break;
        }
        case COMMAND_AUTOFILL: {
            performAutoFill(*gameP);
            success = true;    /*fail condition checked in isLegalMove*/
            break;
        }
        case COMMAND_RESET: {
            performReset(*gameP);
            success = true; /*fail condition checked in isLegalMove*/
            break;
        }
        case COMMAND_EXIT: { /*TODO: nir make sure that free all resorces*/
            terminateProgram(*gameP, FC_SUCCESS);
            break;
        }
        case COMMAND_INVALID: {
            printf("Unreachable Code Error\n");
            terminateProgram(*gameP, FC_UNEXPECTED_ERROR);
        }
    }

    if (success == true &&
        (input.command == COMMAND_SOLVE ||
         input.command == COMMAND_EDIT ||
         input.command == COMMAND_SET ||
         input.command == COMMAND_AUTOFILL ||
         input.command == COMMAND_UNDO ||
         input.command == COMMAND_REDO ||
         input.command == COMMAND_GENERATE ||
         input.command == COMMAND_GUESS ||
         input.command == COMMAND_RESET)) {
        printBoard(*gameP);
    }

}

