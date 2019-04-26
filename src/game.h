#ifndef HW2_GAME_H
#define HW2_GAME_H

#include "main_aux.h"

/*Game module - responsible for managing all the logic of the game*/

Bool askUserForNextTurn(Input *input);

void executeCommand(Game **gameP, Input input);

Bool checkLegalInput(Game *game, Input input);

#endif
