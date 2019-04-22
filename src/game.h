#ifndef HW2_GAME_H
#define HW2_GAME_H

#include "main_aux.h"

void hint(Game *game, Coordinate coordinate);

void validate(Game *game);

Bool setCoordinate(Game *game, Input input);

Bool isCoordinateFixed(Game *game, Coordinate coordinate);

Bool isCoordinateEmpty(Game *game, Coordinate coordinate);

#endif
