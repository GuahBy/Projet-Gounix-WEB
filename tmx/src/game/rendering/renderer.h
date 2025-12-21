#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "../core/types.h"
#include <SDL2/SDL.h>
#include <tmx.h>

/* Variables globales pour le rendu */
extern SDL_Renderer *ren;
extern TTF_Font *global_font;

/* Initialisation et chargement */
void *SDL_tex_loader(const char *path);

/* Fonctions de rendu de tiles et layers */
void set_color(int color);
void draw_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               unsigned int dx, unsigned int dy, float opacity, unsigned int flags, Camera *cam);
void draw_layer(tmx_map *map, tmx_layer *layer, Camera *cam);
void draw_all_layers(tmx_map *map, tmx_layer *layer, Camera *cam);
void render_map(tmx_map *map, Camera *cam);

/* Rendu des entités */
void render_player(Player *p, SDL_Renderer *renderer, Camera *cam);
void render_pnj(PNJ *pnj, SDL_Renderer *renderer, Camera *cam);

/* Utilitaires de rendu */
void printf_2D(int x, int y, const char *format, ...);
void draw_status_bar_label(SDL_Renderer *renderer, int x, int y, int value, const char *label, SDL_Color color);
void render_stats(Player *p);

#endif /* GAME_RENDERER_H */
