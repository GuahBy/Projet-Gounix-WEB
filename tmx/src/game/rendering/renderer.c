#include "renderer.h"
#include "../core/constants.h"
#include <stdio.h>
#include <stdarg.h>
#include <SDL2/SDL_image.h>

/* Variables globales */
SDL_Renderer *ren = NULL;
TTF_Font *global_font = NULL;

void *SDL_tex_loader(const char *path)
{
    return IMG_LoadTexture(ren, path);
}

void set_color(int color)
{
    tmx_col_bytes col = tmx_col_to_bytes(color);
    SDL_SetRenderDrawColor(ren, col.r, col.g, col.b, col.a);
}

void draw_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               unsigned int dx, unsigned int dy, float opacity, unsigned int flags, Camera *cam)
{
    SDL_Rect src = {sx, sy, sw, sh};
    SDL_Rect dest = {dx - cam->x, dy - cam->y, sw * cam->zoom, sh * cam->zoom};
    SDL_RenderCopy(ren, (SDL_Texture *)image, &src, &dest);
}

void draw_layer(tmx_map *map, tmx_layer *layer, Camera *cam)
{
    for (unsigned long i = 0; i < map->height; i++)
    {
        for (unsigned long j = 0; j < map->width; j++)
        {
            unsigned int gid = layer->content.gids[i * map->width + j] & TMX_FLIP_BITS_REMOVAL;
            if (map->tiles[gid])
            {
                tmx_tile *tile = map->tiles[gid];
                tmx_tileset *ts = tile->tileset;
                void *image = tile->image ? tile->image->resource_image : ts->image->resource_image;
                draw_tile(image, tile->ul_x, tile->ul_y, ts->tile_width, ts->tile_height,
                          j * ts->tile_width, i * ts->tile_height, layer->opacity, 0, cam);
            }
        }
    }
}

void draw_all_layers(tmx_map *map, tmx_layer *layer, Camera *cam)
{
    while (layer)
    {
        if (layer->visible && layer->type == L_LAYER)
            draw_layer(map, layer, cam);
        layer = layer->next;
    }
}

void render_map(tmx_map *map, Camera *cam)
{
    set_color(map->backgroundcolor);
    SDL_RenderClear(ren);
    draw_all_layers(map, map->ly_head, cam);
}

void render_player(Player *p, SDL_Renderer *renderer, Camera *cam)
{
    SDL_Rect r = {p->x - cam->x, p->y - cam->y, p->size * cam->zoom, p->size * cam->zoom};
    SDL_RenderCopy(renderer, p->texture, NULL, &r);
}

void render_pnj(PNJ *pnj, SDL_Renderer *renderer, Camera *cam)
{
    SDL_Rect rect;
    rect.x = pnj->x - cam->x;
    rect.y = pnj->y - cam->y;
    rect.w = pnj->size * cam->zoom;
    rect.h = pnj->size * cam->zoom;
    SDL_RenderCopy(renderer, pnj->texture, NULL, &rect);
}

void printf_2D(int x, int y, const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surf = TTF_RenderText_Solid(global_font, buffer, color);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect rect = {x, y, surf->w, surf->h};
    SDL_RenderCopy(ren, tex, NULL, &rect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void draw_status_bar_label(SDL_Renderer *renderer, int x, int y, int value, const char *label, SDL_Color color)
{
    int bar_width = 150;
    int bar_height = 20;

    // Fond de la barre
    SDL_Rect bg = {x, y, bar_width, bar_height};
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderFillRect(renderer, &bg);

    // Remplissage de la barre selon la valeur
    SDL_Rect fill = {x + 2, y + 2, (bar_width - 4) * value / 100, bar_height - 4};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &fill);

    // Contour blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bg);

    // Texte sur fond blanc
    char label_text[32];
    snprintf(label_text, sizeof(label_text), "%s : %d", label, value);

    SDL_Color text_color = {0, 0, 0, 255}; // Texte noir
    SDL_Surface *text_surf = TTF_RenderText_Shaded(global_font, label_text, text_color, (SDL_Color){255, 255, 255});
    SDL_Texture *text_tex = SDL_CreateTextureFromSurface(ren, text_surf);
    SDL_Rect text_rect = {x + bar_width + 10, y, text_surf->w, text_surf->h};
    SDL_RenderCopy(renderer, text_tex, NULL, &text_rect);
    SDL_FreeSurface(text_surf);
    SDL_DestroyTexture(text_tex);
}

void render_stats(Player *p)
{
    draw_status_bar_label(ren, 20, 20, p->stats.pv, "PV", (SDL_Color){255, 0, 0});
    draw_status_bar_label(ren, 20, 50, p->stats.faim, "Faim", (SDL_Color){255, 165, 0});
    draw_status_bar_label(ren, 20, 80, p->stats.energie, "Energie", (SDL_Color){0, 128, 255});

    // Position du fond + texte
    int heure_x = DISPLAY_W - 150;
    int heure_y = 20;
    int heure_w = 130;
    int heure_h = 28;

    // Fond blanc semi-transparent
    SDL_Rect rect_heure = {heure_x, heure_y, heure_w, heure_h};
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 220);
    SDL_RenderFillRect(ren, &rect_heure);

    // Texte noir par-dessus
    char heure_str[32];
    sprintf(heure_str, "Heure : %02dh00", p->heure_du_jour);
    SDL_Color noir = {0, 0, 0, 255};
    SDL_Surface *surf = TTF_RenderText_Solid(global_font, heure_str, noir);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect pos = {heure_x + 10, heure_y + 5, surf->w, surf->h};
    SDL_RenderCopy(ren, tex, NULL, &pos);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}
