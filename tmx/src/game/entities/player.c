#include "player.h"
#include "../core/constants.h"
#include "../rendering/ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Déclaration externe de printf_2D (définie dans renderer.c)
extern void printf_2D(int x, int y, const char *format, ...);

Player *init_player(SDL_Renderer *renderer, const char *sprite_path)
{
    Player *p = malloc(sizeof(Player));
    if (!p)
    {
        fprintf(stderr, "Erreur allocation joueur\n");
        return NULL;
    }

    SDL_Surface *surf = IMG_Load(sprite_path);
    p->texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    p->x = DISPLAY_W / 2;
    p->y = DISPLAY_H / 2;
    p->speed = PLAYER_SPEED;
    p->size = PLAYER_SIZE;
    p->stats = (Stats){100, 100, 100};
    p->a_force_sur_nourriture = 0;
    p->faim_warning_affiche = 0;
    p->energie_warning_affiche = 0;
    p->mort_affichee = 0;
    p->dernier_sommeil = 0;
    p->inventaire.a_livre = 0;
    p->inventaire.a_stylo = 0;
    p->inventaire.a_tampon = 0;
    p->inventaire.a_manuscrit = 0;
    p->inventaire.a_autorisation = 0;
    p->heure_du_jour = 9;
    p->dernier_tick_heure = SDL_GetTicks();
    p->quete.etape = 0;
    p->quete.parle_a_mamie = 0;
    p->quete.a_trouve_livre = 0;
    p->quete.parle_a_kevin = 0;
    p->quete.a_donne_autographe = 0;
    p->quete.a_tampon = 0;
    p->quete.a_manuscrit = 0;
    p->quete.a_autorisation = 0;
    p->quete.a_declenche_conference = 0;
    p->gain_nourriture = 10;
    p->gain_sommeil = 30;

    return p;
}

void update_and_render_stats(Player *p)
{
    static Uint32 last_update = 0;
    if (SDL_GetTicks() - last_update > 2000)
    {
        last_update = SDL_GetTicks();
        if (p->stats.faim > 0)
            p->stats.faim--;
        if (p->stats.energie > 0)
            p->stats.energie--;
        if (p->stats.faim == 0 || p->stats.energie == 0)
        {
            if (p->stats.pv > 0)
                p->stats.pv--;
        }
    }
}

void reload_player_texture(Player *p, SDL_Renderer *renderer, const char *sprite_path)
{
    SDL_Surface *surface = IMG_Load(sprite_path);
    if (!surface)
    {
        fprintf(stderr, "Erreur IMG_Load : %s\n", IMG_GetError());
        exit(1);
    }

    if (p->texture != NULL)
    {
        SDL_DestroyTexture(p->texture); // évite fuite mémoire
    }

    p->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!p->texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s\n", SDL_GetError());
        exit(1);
    }
}

int warningDeath(Player *player, SDL_Renderer *renderer, TTF_Font *font)
{
    // FAIM
    if (player->stats.faim < 30 && !player->faim_warning_affiche)
    {
        afficher_dialogue_box("Je commence à avoir faim...", renderer, font);
        SDL_RenderPresent(renderer);
        SDL_Delay(900);
        player->faim_warning_affiche = 1;
    }
    else if (player->stats.faim > 60 && player->faim_warning_affiche)
    {
        player->faim_warning_affiche = 0;
    }

    // eNERGIE
    if (player->stats.energie < 30 && !player->energie_warning_affiche)
    {
        afficher_dialogue_box("Rfff... Je commence a etre epuise...", renderer, font);
        SDL_RenderPresent(renderer);
        SDL_Delay(900);
        player->energie_warning_affiche = 1;
    }
    else if (player->stats.energie > 60 && player->energie_warning_affiche)
    {
        player->energie_warning_affiche = 0;
    }

    // MORT
    if (player->stats.pv <= 0 && !player->mort_affichee)
    {
        player->mort_affichee = 1;

        // ecran Game Over
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Rect box = {200, 250, 400, 100};
        SDL_SetRenderDrawColor(renderer, 80, 0, 0, 255);
        SDL_RenderFillRect(renderer, &box);
        printf_2D(280, 280, "GAME OVER - Jean-Mi est mort");
        SDL_RenderPresent(renderer);
        SDL_Delay(3000);

        return 1; // Signale à game_loop que le joueur est mort
    }
    else if (player->stats.pv > 0)
    {
        player->mort_affichee = 0;
    }

    return 0;
}
