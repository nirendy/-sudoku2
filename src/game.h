#ifndef HW2_GAME_H
#define HW2_GAME_H

#include "main_aux.h"

/*Game module - responsible for managing all the logic of the game*/

Bool isCoordinateFixed(Game *game, Coordinate coordinate);

Bool isCoordinateEmpty(Game *game, Coordinate coordinate);

void setCoordinate(Game *game, Input input);

void hint(Game *game, Input input);

void validate(Game *game);

void clearGame(Game *game);

#endif
