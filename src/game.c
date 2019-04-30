#include "game.h"
#include "solver.h"
#include "parser.h"
#include "linked_list.h"
#include "file_handler.h"
#include "lp_solver.h"
#include "game_aux.h"

/*Game module - responsible for managing the game flow and the commands' execution*/

/* Perform Commands
 *
 * functions with Bool as return value return whether or not the operation was successful
 * */

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
        setGlobalNodeNextToNull();
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
        setGlobalNodeNextToNull();
        return true;
    }
    return false;
}

void performMarkErrors(Input input) {
    if (input.value == 0) { g_markError = false; }
    else if (input.value == 1) { g_markError = true; }
    else { printError(EUnreachableCode); }
}

void performSet(Game *game, Input input) {

    Input redoInput, undoInput;
    clearListFromNode(g_curNode->next);
    setGlobalNodeNextToNull();
    updateRedoUndoInputsAfterSingleSet(game, input, &redoInput, &undoInput);
    insertInputsToList(&redoInput, &undoInput, 1);
    setCoordinate(game, input);
}

void performValidate(Game *game) {
    if (isSolvableBoard(game->user_matrix)) {
        printPrompt(PValidateSuccess, 0);
    } else {
        printPrompt(PValidateFailed, 0);
    }
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

void performRedo(Game *game) {
    Input input;
    DataNode *currDataNode;

    printPrompt(PPerformedChanges, 0);
    g_curNode = g_curNode->next;
    currDataNode = g_curNode->currDataNode;

    currDataNode = getFirstDataNode(currDataNode)->next;
    while (currDataNode != NULL) {
        input = currDataNode->redoInput;
        printChange(input.coordinate.i, input.coordinate.j, input.value);
        setCoordinate(game, input);
        currDataNode = currDataNode->next;
    }
}

Bool performGuess(Game *game, Input input) {

    Board solutionBoard = createBoard();
    if (!guessBoard(game->user_matrix, solutionBoard, input.threshold)) {
        destroyBoard(solutionBoard, g_gameDim);
        printPrompt(PGuessFailure,0);
        printBoard(game);
        return false;
    }

    clearListFromNode(g_curNode->next);
    setGlobalNodeNextToNull();

    /*print the performed changes if there are any*/
    if (updateHistoryList(game, solutionBoard) > 0) { /*destroys newBoard*/
        performUndo(game, g_curNode->currDataNode, false);
        performRedo(game);
    } else {
        printPrompt(PGuessFailure,0);;
    }
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

    /*At this stage newBoard is fully and legally solved,
     * and we assume that when moving on to the next steps */

    /*step 2 - clear cells from the board */
    if (numToClear > 0) {
        cellsToClear = (Coordinate *) smartMalloc(numToClear * sizeof(Coordinate));
        chooseCellsToClear(newBoard, cellsToClear, numToClear);
        clearRandomCells(newBoard, cellsToClear, numToClear);
        free(cellsToClear);
    }

    /*step 3 - perform changes and update the redo/undo list */
    clearListFromNode(g_curNode->next);
    setGlobalNodeNextToNull();
    updateHistoryList(game, newBoard); /*destroys newBoard*/
    return true;

}

void performSave(Game *game, Input input) {
    Board solutionBoard = createBoard();

    if (g_mode == Edit && !fillSolutionMatrix(game->user_matrix, solutionBoard)) {
        printError(EFUnsolvableBoard);
    } else {
        saveGameToFile(input.path, game);
    }
    destroyBoard(solutionBoard, g_gameDim);
}

void performHint(Game *game, Input input) {

    Board solutionBoard;
    solutionBoard = createBoard();

    if (fillSolutionMatrix(game->user_matrix, solutionBoard)) {
        printPrompt(PHint, solutionBoard[input.coordinate.i][input.coordinate.j]);
    } else {
        printError(EFUnsolvableBoard);
    }

    destroyBoard(solutionBoard, g_gameDim);

}

void performGuessHint(Game *game, Input input) {
    guessHint(game->user_matrix, input.coordinate);

}

void performNumSolutions(Game *game) {
    printPrompt(
            PNumSolutionsOutput,
            getNumOfPossibleSolutions(game->user_matrix)
    );
}

void performAutoFill(Game *game) {

    Board newBoard = createBoard();
    copyBoard(newBoard, game->user_matrix);
    fillObviousValues(game, newBoard);
    clearListFromNode(g_curNode->next);
    setGlobalNodeNextToNull();
    /*print the performed changes if there are any*/
    if (updateHistoryList(game, newBoard) > 0) { /*destroys newBoard*/
        performUndo(game, g_curNode->currDataNode, false);
        performRedo(game);
    } else {
        printPrompt(PNoObviousCells, 0);
    }
}

void performReset(Game *game) {
    while (!g_curNode->isFirst) {
        performUndo(game, g_curNode->currDataNode, false);
    }
}

/* Game flow */
/**
 *
 * @param input
 * @return prints the request for command, and return whether or not the command parse was successful
 */
Bool askUserForNextTurn(Input *input) {
    printPrompt(PNextCommand, 0);
    return parseCommand(input);
}

/**
 *  checking the legality of the input before moving on to the next stage of the command execution
 * @param game
 * @param input
 * @return whether the input is legal
 */
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
                printf("parameter must be a binary number: 0 or 1\n");
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
                printf("parameter must be smaller or equal than the number of empty cells: %d\n", numOfEmptyCells);
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
                printf("parameter must be an integer number between 1 and %d\n", g_gameDim.N);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.coordinate.j >= 0 && input.coordinate.j <= g_gameDim.N - 1)) {
                printError(EInvalidSecondParam);
                printf("parameter must be an integer number between 1 and %d\n", g_gameDim.N);
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
                printf("parameter must be an integer number between 1 and %d\n", g_gameDim.N);
                return false;
            }

            /*Second Parameter Check*/
            if (!(input.coordinate.j >= 0 && input.coordinate.j <= g_gameDim.N - 1)) {
                printError(EInvalidSecondParam);
                printf("parameter must be an integer number between 1 and %d\n", g_gameDim.N);
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
            printError(EUnreachableCode);
        }


    }
    return false;
}

void executeCommand(Game **gameP, Input input) {
    Bool success = false; /* successfully changed board */

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
            performRedo(*gameP);
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
        case COMMAND_EXIT: {
            terminateProgram(*gameP);
            break;
        }
        case COMMAND_INVALID: {
            printError(EUnreachableCode);
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

    /*if any board-changing command was performed in solve mode we check if she filled the board
     * and act accordingly whether the solution is valid or not*/
    if (success == true && g_mode == Solve &&
        (input.command == COMMAND_SOLVE ||
         input.command == COMMAND_SET ||
         input.command == COMMAND_GUESS ||
         input.command == COMMAND_UNDO ||
         input.command == COMMAND_REDO ||
         input.command == COMMAND_AUTOFILL ||
         input.command == COMMAND_RESET
        )) {
        actIfBoardFullAndValid(*gameP);
    }

}

