#include "interactions.h"
#include "map.h"
#include "../core/constants.h"
#include "../rendering/ui.h"
#include "../rendering/renderer.h"
#include "../entities/pnj.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

void check_interaction_objets_event(Player *p, tmx_map *map, PNJ **pnjs, int nb_pnjs, SDL_Renderer *renderer, TTF_Font *font)
{
    tmx_layer *layer = map->ly_head;
    while (layer)
    {
        if (layer->type == L_OBJGR)
        {
            tmx_object *obj = layer->content.objgr->head;
            while (obj)
            {
                if (obj->visible && obj->obj_type == OT_SQUARE && obj->name)
                {
                    int dx = (obj->x + obj->width / 2) - p->x;
                    int dy = (obj->y + obj->height / 2) - p->y;
                    int distance = dx * dx + dy * dy;
                    if (distance < 900)
                    { // Proximite ≈ 30 pixels
                        if (strcmp(obj->name, "nourriture") == 0)
                        {
                            interaction_nourriture(p, renderer, font);
                        }
                        if (strcmp(obj->name, "lit") == 0)
                        {
                            interaction_lit(p, renderer, map, pnjs, nb_pnjs, font);
                        }
                        if (strcmp(obj->name, "livre") == 0)
                        {
                            if (p->quete.parle_a_mamie && p->quete.etape == 1 && !p->inventaire.a_livre)
                            {
                                // Ramassage du livre
                                p->inventaire.a_livre = 1;
                                p->quete.a_trouve_livre = 1;
                                p->quete.etape = 2;

                                afficher_dialogue_box("Tu as trouve le livre de Poincare.", renderer, font);
                                SDL_RenderPresent(renderer);
                                SDL_Delay(1000);
                                afficher_dialogue_box("Je dois organiser une conference a la NESI pour rehabiliter Poincare.", renderer, font);
                                SDL_RenderPresent(renderer);
                                SDL_Delay(1000);
                                afficher_dialogue_box("Kevin connait du monde dans l'administration...", renderer, font);
                                SDL_RenderPresent(renderer);
                                SDL_Delay(1000);
                            }
                            else if (p->inventaire.a_livre)
                            {
                                afficher_dialogue_box("Tu as deja pris le livre.", renderer, font);
                                SDL_RenderPresent(renderer);
                                SDL_Delay(1000);
                            }
                            else
                            {
                                afficher_dialogue_box("Ce livre semble ancien... mais je ne vois pas quoi en faire pour le moment.", renderer, font);
                                SDL_RenderPresent(renderer);
                                SDL_Delay(1000);
                            }
                        }

                        if (strcmp(obj->name, "stylo") == 0)
                        {
                            if (!p->quete.parle_a_kevin)
                            {
                                afficher_dialogue_box("Ce n est pas le moment de prendre ce stylo...", renderer, font);
                                SDL_RenderPresent(renderer);
                                SDL_Delay(900);
                                return;
                            }

                            if (!p->inventaire.a_stylo)
                            {
                                p->inventaire.a_stylo = 1;
                                afficher_dialogue_box("Tu recuperes un vieux stylo...", renderer, font);
                                SDL_RenderPresent(renderer);
                                SDL_Delay(900);
                            }
                            else
                            {
                                afficher_dialogue_box("Tu as deja pris ce stylo.", renderer, font);
                                SDL_RenderPresent(renderer);
                                SDL_Delay(900);
                            }
                        }
                        if (strcmp(obj->name, "tampon") == 0)
                        {
                            if (!p->inventaire.a_tampon)
                            {
                                p->inventaire.a_tampon = 1;
                                afficher_dialogue_box("Tu recuperes un tampon dore...", renderer, font);
                                SDL_RenderPresent(renderer);
                                SDL_Delay(900);
                            }
                        }
                    }
                }
                obj = obj->next;
            }
        }
        layer = layer->next;
    }
}

void interaction_nourriture(Player *p, SDL_Renderer *renderer, TTF_Font *font)
{
    // Si on est dans la zone critique
    if (p->stats.faim >= 90)
    {
        if (!p->a_force_sur_nourriture)
        {
            // Premier avertissement dès 90
            p->a_force_sur_nourriture = 1;
            afficher_dialogue_box("Je suis plein... si je continue, je vais exploser !", renderer, font);
            SDL_RenderPresent(renderer);
            SDL_Delay(900);
            return;
        }
        else if (p->stats.faim >= 100)
        {
            // Deuxième fois à 100 → mort
            afficher_dialogue_box("Tu forces trop, tu fais une indigestion...", renderer, font);
            SDL_RenderPresent(renderer);
            SDL_Delay(900);
            p->stats.pv = 0;
            return;
        }
    }

    // Manger normalement
    p->stats.faim += p->gain_nourriture;
    if (p->stats.faim > 100)
        p->stats.faim = 100;
    afficher_dialogue_box("Miam...!", renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(900);

    // On reset le flag si on est redescendu
    if (p->stats.faim < 90)
        p->a_force_sur_nourriture = 0;
}

void interaction_lit(Player *p, SDL_Renderer *renderer, tmx_map *map, PNJ **pnjs, int nb_pnjs, TTF_Font *font)
{
    int now = SDL_GetTicks();

    // Anti-spam
    if (now - p->dernier_sommeil < 4000)
    {
        afficher_dialogue_box("Tu viens juste de te reposer.", renderer, font);
        SDL_RenderPresent(renderer);
        SDL_Delay(900);
        return;
    }

    p->dernier_sommeil = now;

    if (p->stats.energie >= 100)
    {
        afficher_dialogue_box("Je suis deja en pleine forme !", renderer, font);
        SDL_RenderPresent(renderer);
        SDL_Delay(900);
        return;
    }

    // Message repos
    afficher_dialogue_box("Tu te reposes un moment...", renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(900);

    // ecran noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

    // Gagner de l'energie
    p->stats.energie += p->gain_sommeil;
    if (p->stats.energie > 100)
        p->stats.energie = 100;

    // Rafraîchissement map
    Camera cam = {p->x - DISPLAY_W / 2, p->y - DISPLAY_H / 2, INITIAL_ZOOM};
    render_map(map, &cam);
    render_player(p, renderer, &cam);
    for (int i = 0; i < nb_pnjs; i++)
    {
        render_pnj(pnjs[i], renderer, &cam);
    }
    render_stats(p);
    SDL_RenderPresent(renderer);

    p->heure_du_jour += 4;
    if (p->heure_du_jour >= 24)
        p->heure_du_jour -= 24;
}

void update_player_and_handle_exit(Player *p, int mx, int my, tmx_map **map, SDL_Renderer *renderer, int *current_map_id, PNJ ***pnjs, int *nb_pnjs)
{
    int new_x = p->x + mx * p->speed;
    int new_y = p->y + my * p->speed;
    tmx_layer *layer = (*map)->ly_head;
    while (layer)
    {
        if (layer->type == L_OBJGR)
        {
            tmx_object *obj = layer->content.objgr->head;
            while (obj)
            {
                if (obj->visible && obj->obj_type == OT_SQUARE)
                {
                    int obj_x = obj->x;
                    int obj_y = obj->y;
                    int obj_w = obj->width;
                    int obj_h = obj->height;
                    int px = new_x - p->size / 2;
                    int py = new_y - p->size / 2;

                    if (px < obj_x + obj_w && px + p->size > obj_x &&
                        py < obj_y + obj_h && py + p->size > obj_y)
                    {
                        if (layer->name && strcmp(layer->name, "Collisions") == 0)
                            return;

                        if (*current_map_id == 0 && obj->name && strcmp(obj->name, "Exit") == 0)
                        {
                            liberer_pnjs(*pnjs, *nb_pnjs);
                            tmx_map_free(*map);
                            strcpy(current_map_path, "ressources/maps/Village_2_Gounix_mini.tmx");
                            *map = tmx_load(current_map_path);
                            *pnjs = charger_pnjs_depuis_tiled(*map, renderer, nb_pnjs);
                            *current_map_id = get_map_id(current_map_path);
                            p->x = 55;
                            p->y = 340;
                            printf("Passage à la carte Village\n");
                            SDL_Delay(250);
                            return;
                        }
                        if (*current_map_id == 2 && obj->name && strcmp(obj->name, "Exit") == 0)
                        {
                            liberer_pnjs(*pnjs, *nb_pnjs);
                            tmx_map_free(*map);
                            strcpy(current_map_path, "ressources/maps/Village_2_Gounix_mini.tmx");
                            *map = tmx_load(current_map_path);
                            *pnjs = charger_pnjs_depuis_tiled(*map, renderer, nb_pnjs);
                            *current_map_id = get_map_id(current_map_path);
                            p->x = 650;
                            p->y = 340;
                            printf("Retour à la carte Village depuis le Bar\n");
                            SDL_Delay(250);
                            return;
                        }
                        if (*current_map_id == 3 && obj->name && strcmp(obj->name, "Exit") == 0)
                        {
                            liberer_pnjs(*pnjs, *nb_pnjs);
                            tmx_map_free(*map);
                            strcpy(current_map_path, "ressources/maps/Village_2_Gounix_mini.tmx");
                            *map = tmx_load(current_map_path);
                            *pnjs = charger_pnjs_depuis_tiled(*map, renderer, nb_pnjs);
                            *current_map_id = get_map_id(current_map_path);
                            p->x = 335;
                            p->y = 180;
                            printf("Retour à la carte Village depuis la Classe\n");
                            SDL_Delay(250);
                            return;
                        }
                        if (*current_map_id == 1 && obj->name)
                        {
                            if (strcmp(obj->name, "ExitMaison") == 0)
                            {
                                liberer_pnjs(*pnjs, *nb_pnjs);
                                tmx_map_free(*map);
                                strcpy(current_map_path, "ressources/maps/Maison_2_Gounix.tmx");
                                *map = tmx_load(current_map_path);
                                *pnjs = charger_pnjs_depuis_tiled(*map, renderer, nb_pnjs);
                                *current_map_id = get_map_id(current_map_path);
                                p->x = 100;
                                p->y = 420;
                                printf("Retour à la carte Maison\n");
                                SDL_Delay(250);
                                return;
                            }
                            else if (strcmp(obj->name, "ExitBar") == 0)
                            {
                                liberer_pnjs(*pnjs, *nb_pnjs);
                                tmx_map_free(*map);
                                strcpy(current_map_path, "ressources/maps/Bar_2_Gounix.tmx");
                                *map = tmx_load(current_map_path);
                                *pnjs = charger_pnjs_depuis_tiled(*map, renderer, nb_pnjs);
                                *current_map_id = get_map_id(current_map_path);
                                p->x = 230;
                                p->y = 350;
                                printf("Aller à la carte Bar\n");
                                SDL_Delay(250);
                                return;
                            }
                            else if (strcmp(obj->name, "ExitEcole") == 0)
                            {
                                liberer_pnjs(*pnjs, *nb_pnjs);
                                tmx_map_free(*map);
                                strcpy(current_map_path, "ressources/maps/Classe_2_Gounix.tmx");
                                *map = tmx_load(current_map_path);
                                *pnjs = charger_pnjs_depuis_tiled(*map, renderer, nb_pnjs);
                                *current_map_id = get_map_id(current_map_path);
                                p->x = 90;
                                p->y = 80;
                                printf("Aller à la carte Classe\n");
                                SDL_Delay(250);
                                return;
                            }
                        }
                    }
                }
                obj = obj->next;
            }
        }
        layer = layer->next;
    }
    p->x = new_x;
    p->y = new_y;
}
