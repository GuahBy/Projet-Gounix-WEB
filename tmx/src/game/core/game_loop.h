#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <SDL2/SDL.h>
#include <tmx.h>
#include "types.h"

// Boucle de jeu principale
void game_loop(SDL_Renderer *renderer, tmx_map **map, Player *player, int *current_map_id, TTF_Font *font);

#endif // GAME_LOOP_H
