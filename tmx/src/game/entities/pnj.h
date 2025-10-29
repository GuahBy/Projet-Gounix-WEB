#ifndef PNJ_H
#define PNJ_H

#include <SDL2/SDL.h>
#include <tmx.h>
#include "../core/types.h"

// Données PNJ
extern PNJ_Info pnj_infos[];
extern const int NB_PNJ_INFOS;

// Fonctions PNJ
PNJ *init_pnj(SDL_Renderer *renderer, const char *sprite_path, int dx, int dy);
PNJ *init_pnj_from_name(const char *name, int x, int y, SDL_Renderer *renderer);
int is_near_pnj(Player *player, PNJ *pnj, float dist);
PNJ **charger_pnjs_depuis_tiled(tmx_map *map, SDL_Renderer *renderer, int *nb_pnjs);
void liberer_pnjs(PNJ **pnjs, int nb);
const char *get_dialogue_pnj(Player *player, PNJ *pnj, SDL_Renderer *renderer, TTF_Font *font);
const char *get_random_dialogue(PNJ *pnj, const char *name);

#endif // PNJ_H
