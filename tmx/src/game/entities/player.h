#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include "../core/types.h"

// Fonctions liées au joueur
Player *init_player(SDL_Renderer *renderer, const char *sprite_path);
void update_and_render_stats(Player *p);
void reload_player_texture(Player *p, SDL_Renderer *renderer, const char *sprite_path);
int warningDeath(Player *player, SDL_Renderer *renderer, TTF_Font *font);

#endif // PLAYER_H
