#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include <SDL2/SDL.h>
#include <tmx.h>
#include "../core/types.h"

// Fonctions d'interactions
void check_interaction_objets_event(Player *p, tmx_map *map, PNJ **pnjs, int nb_pnjs, SDL_Renderer *renderer, TTF_Font *font);
void interaction_nourriture(Player *p, SDL_Renderer *renderer, TTF_Font *font);
void interaction_lit(Player *p, SDL_Renderer *renderer, tmx_map *map, PNJ **pnjs, int nb_pnjs, TTF_Font *font);
void update_player_and_handle_exit(Player *p, int mx, int my, tmx_map **map, SDL_Renderer *renderer, int *current_map_id, PNJ ***pnjs, int *nb_pnjs);

#endif // INTERACTIONS_H
