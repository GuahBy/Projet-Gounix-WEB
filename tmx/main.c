#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <tmx.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

/* Includes des modules du jeu */
#include "src/game/core/types.h"
#include "src/game/core/constants.h"
#include "src/game/rendering/renderer.h"
#include "src/game/rendering/ui.h"
#include "src/game/entities/player.h"
#include "src/game/entities/pnj.h"
#include "src/game/world/map.h"
#include "src/game/world/interactions.h"
#include "src/game/core/game_loop.h"
#include "src/game/core/save_load.h"

/**
 * Point d'entrée principal du jeu Jean-Mi Simulator
 */
int main(int argc, char **argv)
{
    /* Initialisation SDL */
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window *win = SDL_CreateWindow("Jean-Mi Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_W, DISPLAY_H, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    ren = renderer;
    global_font = TTF_OpenFont("ressources/font/Pixellettersfull-BnJ5.ttf", 24);

    if (!win || !renderer || !global_font)
    {
        fprintf(stderr, "Erreur lors de l'initialisation SDL :\n");
        if (!win)
            fprintf(stderr, " - SDL_CreateWindow a échoué : %s\n", SDL_GetError());
        if (!renderer)
            fprintf(stderr, " - SDL_CreateRenderer a échoué : %s\n", SDL_GetError());
        if (!global_font)
            fprintf(stderr, " - TTF_OpenFont a échoué : %s\n", TTF_GetError());
        return 1;
    }

    /* Configuration TMX */
    tmx_img_load_func = SDL_tex_loader;
    tmx_img_free_func = (void (*)(void *))SDL_DestroyTexture;

    /* Variables de jeu */
    Player *joueur = NULL;
    tmx_map *map = NULL;
    int current_map_id = 0;
    int running = 1;
    srand(time(NULL));

    /* Boucle principale du menu */
    while (running)
    {
        /* Vider tous les événements résiduels avant de réafficher le menu */
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
        }

        int choice = show_menu(renderer, global_font, joueur);

        if (choice == 0) /* Nouvelle Partie */
        {
            strcpy(current_map_path, "ressources/maps/Maison_2_Gounix.tmx");
            map = tmx_load(current_map_path);
            current_map_id = get_map_id(current_map_path);

            if (!map)
            {
                tmx_perror("Erreur chargement map");
                return 1;
            }

            joueur = init_player(renderer, "ressources/pnj/ginoux1.png");
            intro_box(renderer, global_font);
            game_loop(renderer, &map, joueur, &current_map_id, global_font);
            free(joueur);
            joueur = NULL;
            tmx_map_free(map);
        }
        else if (choice == 1) /* Charger Partie */
        {
            Player *joueur_temp = malloc(sizeof(Player));
            if (!joueur_temp)
            {
                fprintf(stderr, "Erreur malloc joueur\n");
                continue;
            }

            afficher_dialogue_box("Appuie sur 1 ou 2 pour charger une partie.", renderer, global_font);
            SDL_RenderPresent(renderer);

            int slot = 0;
            SDL_Event e;
            while (SDL_WaitEvent(&e))
            {
                if (e.type == SDL_KEYDOWN)
                {
                    if (e.key.keysym.sym == SDLK_1)
                    {
                        slot = 1;
                        break;
                    }
                    if (e.key.keysym.sym == SDLK_2)
                    {
                        slot = 2;
                        break;
                    }
                    if (e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        free(joueur_temp);
                        joueur_temp = NULL;
                        break;
                    }
                }
            }

            if (slot != 0 && joueur_temp)
            {
                const char *filename = (slot == 2) ? "save2.dat" : "save1.dat";

                if (!charger_jeu(joueur_temp, current_map_path, filename))
                {
                    afficher_dialogue_box("Echec du chargement.", renderer, global_font);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(1000);
                    free(joueur_temp);
                    joueur_temp = NULL;
                }
                else
                {
                    /* Recharge texture immédiatement après chargement brut */
                    reload_player_texture(joueur_temp, renderer, "ressources/pnj/ginoux1.png");

                    map = tmx_load(current_map_path);
                    current_map_id = get_map_id(current_map_path);
                    if (!map)
                    {
                        tmx_perror("Erreur chargement map");
                        free(joueur_temp);
                        joueur_temp = NULL;
                        continue;
                    }

                    joueur = joueur_temp;
                    game_loop(renderer, &map, joueur, &current_map_id, global_font);

                    free(joueur);
                    joueur = NULL;
                    tmx_map_free(map);
                }
            }
        }
        else if (choice == 2) /* Quitter */
        {
            running = 0;
        }
    }

    /* Nettoyage */
    TTF_CloseFont(global_font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
