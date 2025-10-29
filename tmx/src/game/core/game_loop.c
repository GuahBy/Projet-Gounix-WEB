#include "game_loop.h"
#include "constants.h"
#include "save_load.h"
#include "../rendering/ui.h"
#include "../rendering/renderer.h"
#include "../entities/player.h"
#include "../entities/pnj.h"
#include "../world/map.h"
#include "../world/interactions.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

void game_loop(SDL_Renderer *renderer, tmx_map **map, Player *player, int *current_map_id, TTF_Font *font)
{
    SDL_Event e;
    Camera cam = {0, 0, INITIAL_ZOOM};
    int move_x = 0, move_y = 0;
    int run = 1;
    int dialogue_index = -1;

    int nb_pnjs = 0;
    PNJ **pnjs = charger_pnjs_depuis_tiled(*map, renderer, &nb_pnjs);

    for (int i = 0; i < nb_pnjs; i++)
    {
        if (!pnjs[i])
        {
            printf("Erreur lors de l'initialisation du PNJ %d\n", i);
            exit(1);
        }
    }

    while (run)
    {
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                run = 0;
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_w:
                case SDLK_UP:
                    move_y = -1;
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    move_y = 1;
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    move_x = -1;
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
                    move_x = 1;
                    break;

                case SDLK_F9:
                {
                    const char *options[] = {
                        "1. Menu Statistiques",
                        "2. Modifier gain nourriture / sommeil",
                        "3. Gestion du temps",
                        "4. Modifier taille PNJ",
                        "5. Gestion de l'inventaire",
                        "6. Teleportation",
                        "7. Modifier l'etape de la quete",
                        "8. Voir Etat Actuel",
                        "9. Quitter le mode admin"};
                    const int nb_options = sizeof(options) / sizeof(options[0]);
                    int selected = 0;
                    int admin_loop = 1;
                    SDL_Event e_admin;

                    while (admin_loop)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);

                        for (int i = 0; i < nb_options; i++)
                        {
                            SDL_Color color = (i == selected) ? (SDL_Color){255, 255, 0} : (SDL_Color){255, 255, 255};
                            SDL_Surface *surf = TTF_RenderText_Solid(font, options[i], color);
                            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
                            SDL_Rect pos = {DISPLAY_W / 2 - surf->w / 2, 100 + i * 40, surf->w, surf->h};
                            SDL_RenderCopy(renderer, tex, NULL, &pos);
                            SDL_FreeSurface(surf);
                            SDL_DestroyTexture(tex);
                        }

                        SDL_RenderPresent(renderer);

                        while (SDL_PollEvent(&e_admin))
                        {
                            if (e_admin.type == SDL_KEYDOWN)
                            {
                                if (e_admin.key.keysym.sym == SDLK_UP)
                                    selected = (selected + nb_options - 1) % nb_options;
                                else if (e_admin.key.keysym.sym == SDLK_DOWN)
                                    selected = (selected + 1) % nb_options;
                                else if (e_admin.key.keysym.sym == SDLK_RETURN)
                                {
                                    SDL_Event ev;
                                    switch (selected)
                                    {
                                    case 0:
                                    {
                                        afficher_dialogue_box("Quelle stat modifier ? 1=PV 2=Faim 3=Energie", renderer, font);
                                        SDL_RenderPresent(renderer);

                                        int stat_index = -1;
                                        SDL_Event ev;
                                        while (SDL_WaitEvent(&ev))
                                        {
                                            if (ev.type == SDL_KEYDOWN)
                                            {
                                                if (ev.key.keysym.sym == SDLK_1)
                                                    stat_index = 0;
                                                else if (ev.key.keysym.sym == SDLK_2)
                                                    stat_index = 1;
                                                else if (ev.key.keysym.sym == SDLK_3)
                                                    stat_index = 2;
                                                if (stat_index != -1)
                                                    break;
                                            }
                                        }

                                        afficher_dialogue_box("Nouvelle valeur : 1=10, ..., 9=90", renderer, font);
                                        SDL_RenderPresent(renderer);
                                        int valeur = 0;
                                        while (SDL_WaitEvent(&ev))
                                        {
                                            if (ev.type == SDL_KEYDOWN &&
                                                ev.key.keysym.sym >= SDLK_1 && ev.key.keysym.sym <= SDLK_9)
                                            {
                                                valeur = (ev.key.keysym.sym - SDLK_0) * 10;
                                                break;
                                            }
                                        }

                                        if (stat_index == 0)
                                            player->stats.pv = valeur;
                                        else if (stat_index == 1)
                                            player->stats.faim = valeur;
                                        else
                                            player->stats.energie = valeur;

                                        afficher_dialogue_box("Stat mise a jour !", renderer, font);
                                        SDL_RenderPresent(renderer);
                                        SDL_Delay(1000);
                                        break;
                                    }

                                    case 1:
                                    {
                                        afficher_dialogue_box("Modifier le gain :\n1 = Nourriture\n2 = Sommeil", renderer, font);
                                        SDL_RenderPresent(renderer);

                                        int choix = 0;
                                        SDL_Event ev;
                                        while (SDL_WaitEvent(&ev))
                                        {
                                            if (ev.type == SDL_KEYDOWN)
                                            {
                                                if (ev.key.keysym.sym == SDLK_1)
                                                    choix = 1;
                                                else if (ev.key.keysym.sym == SDLK_2)
                                                    choix = 2;
                                                if (choix != 0)
                                                    break;
                                            }
                                        }

                                        afficher_dialogue_box("Entrer nouvelle valeur : 1=10, ..., 9=90", renderer, font);
                                        SDL_RenderPresent(renderer);

                                        int gain = 0;
                                        while (SDL_WaitEvent(&ev))
                                        {
                                            if (ev.type == SDL_KEYDOWN &&
                                                ev.key.keysym.sym >= SDLK_1 && ev.key.keysym.sym <= SDLK_9)
                                            {
                                                gain = (ev.key.keysym.sym - SDLK_0) * 10;
                                                break;
                                            }
                                        }

                                        if (choix == 1)
                                            player->gain_nourriture = gain;
                                        else
                                            player->gain_sommeil = gain;

                                        char msg[64];
                                        sprintf(msg, "Gain %s mis a %d", (choix == 1 ? "nourriture" : "sommeil"), gain);
                                        afficher_dialogue_box(msg, renderer, font);
                                        SDL_RenderPresent(renderer);
                                        SDL_Delay(1000);
                                        break;
                                    }
                                    case 2:
                                    {
                                        afficher_dialogue_box("1 = Avancer l'heure\n2 = Reculer l'heure", renderer, font);
                                        SDL_RenderPresent(renderer);

                                        int action = 0;
                                        SDL_Event ev;
                                        while (SDL_WaitEvent(&ev))
                                        {
                                            if (ev.type == SDL_KEYDOWN)
                                            {
                                                if (ev.key.keysym.sym == SDLK_1)
                                                    action = 1;
                                                else if (ev.key.keysym.sym == SDLK_2)
                                                    action = 2;
                                                if (action != 0)
                                                    break;
                                            }
                                        }

                                        afficher_dialogue_box("Combien d'heures ? (1 a 9)", renderer, font);
                                        SDL_RenderPresent(renderer);

                                        int heure_modif = 0;
                                        while (SDL_WaitEvent(&ev))
                                        {
                                            if (ev.type == SDL_KEYDOWN &&
                                                ev.key.keysym.sym >= SDLK_1 && ev.key.keysym.sym <= SDLK_9)
                                            {
                                                heure_modif = ev.key.keysym.sym - SDLK_0;
                                                break;
                                            }
                                        }

                                        if (action == 1)
                                            player->heure_du_jour = (player->heure_du_jour + heure_modif) % 24;
                                        else
                                            player->heure_du_jour = (player->heure_du_jour - heure_modif + 24) % 24;

                                        char heure_msg[64];
                                        sprintf(heure_msg, "Nouvelle heure : %dh", player->heure_du_jour);
                                        afficher_dialogue_box(heure_msg, renderer, font);
                                        SDL_RenderPresent(renderer);
                                        SDL_Delay(1000);
                                        break;
                                    }

                                    case 3:
                                    {
                                        afficher_dialogue_box("Choisir taille PNJ : 1=10 ... 9=90", renderer, font);
                                        SDL_RenderPresent(renderer);
                                        int taille = 0;
                                        while (SDL_WaitEvent(&ev))
                                        {
                                            if (ev.type == SDL_KEYDOWN &&
                                                ev.key.keysym.sym >= SDLK_1 && ev.key.keysym.sym <= SDLK_9)
                                            {
                                                taille = (ev.key.keysym.sym - SDLK_0) * 10;
                                                break;
                                            }
                                        }

                                        for (int i = 0; i < nb_pnjs; i++)
                                        {
                                            if (pnjs[i])
                                                pnjs[i]->size = taille;
                                        }

                                        char msg[64];
                                        sprintf(msg, "Taille PNJ mise a %d", taille);
                                        afficher_dialogue_box(msg, renderer, font);
                                        SDL_RenderPresent(renderer);
                                        SDL_Delay(1000);
                                        break;
                                    }
                                    case 4:
                                    {
                                        const char *options[] = {
                                            "1. Inverser Livre",
                                            "2. Inverser Stylo",
                                            "3. Inverser Tampon",
                                            "4. Inverser Manuscrit",
                                            "5. Inverser Autorisation",
                                            "6. Remplir tout l'inventaire",
                                            "7. Vider tout l'inventaire",
                                            "8. Retour"};
                                        const int nb = 8;
                                        int selected = 0;
                                        int inv_loop = 1;
                                        SDL_Event ev;

                                        while (inv_loop)
                                        {
                                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                            SDL_RenderClear(renderer);

                                            for (int i = 0; i < nb; i++)
                                            {
                                                SDL_Color color = (i == selected) ? (SDL_Color){255, 255, 0} : (SDL_Color){255, 255, 255};
                                                SDL_Surface *surf = TTF_RenderText_Solid(font, options[i], color);
                                                SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
                                                SDL_Rect pos = {DISPLAY_W / 2 - surf->w / 2, 100 + i * 40, surf->w, surf->h};
                                                SDL_RenderCopy(renderer, tex, NULL, &pos);
                                                SDL_FreeSurface(surf);
                                                SDL_DestroyTexture(tex);
                                            }

                                            SDL_RenderPresent(renderer);

                                            while (SDL_PollEvent(&ev))
                                            {
                                                if (ev.type == SDL_KEYDOWN)
                                                {
                                                    if (ev.key.keysym.sym == SDLK_UP)
                                                        selected = (selected + nb - 1) % nb;
                                                    else if (ev.key.keysym.sym == SDLK_DOWN)
                                                        selected = (selected + 1) % nb;
                                                    else if (ev.key.keysym.sym == SDLK_RETURN)
                                                    {
                                                        switch (selected)
                                                        {
                                                        case 0:
                                                            player->inventaire.a_livre ^= 1;
                                                            break;
                                                        case 1:
                                                            player->inventaire.a_stylo ^= 1;
                                                            break;
                                                        case 2:
                                                            player->inventaire.a_tampon ^= 1;
                                                            break;
                                                        case 3:
                                                            player->inventaire.a_manuscrit ^= 1;
                                                            break;
                                                        case 4:
                                                            player->inventaire.a_autorisation ^= 1;
                                                            break;
                                                        case 5:
                                                            player->inventaire.a_livre = 1;
                                                            player->inventaire.a_stylo = 1;
                                                            player->inventaire.a_tampon = 1;
                                                            player->inventaire.a_manuscrit = 1;
                                                            player->inventaire.a_autorisation = 1;
                                                            break;
                                                        case 6:
                                                            player->inventaire.a_livre = 0;
                                                            player->inventaire.a_stylo = 0;
                                                            player->inventaire.a_tampon = 0;
                                                            player->inventaire.a_manuscrit = 0;
                                                            player->inventaire.a_autorisation = 0;
                                                            break;
                                                        case 7:
                                                            inv_loop = 0;
                                                            break;
                                                        }
                                                    }
                                                    else if (ev.key.keysym.sym == SDLK_ESCAPE)
                                                    {
                                                        inv_loop = 0;
                                                    }
                                                }
                                            }

                                            SDL_Delay(80);
                                        }
                                    }
                                    break;
                                    case 5:
                                    {
                                        const char *destinations[] = {
                                            "1. Aller a la Maison",
                                            "2. Aller a l'Ecole",
                                            "3. Aller au Village",
                                            "4. Aller au Bar",
                                            "5. Retour"};
                                        const int nb = 5;
                                        int selected = 0;
                                        int tp_loop = 1;
                                        SDL_Event ev;

                                        while (tp_loop)
                                        {
                                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                            SDL_RenderClear(renderer);

                                            for (int i = 0; i < nb; i++)
                                            {
                                                SDL_Color color = (i == selected) ? (SDL_Color){255, 255, 0} : (SDL_Color){255, 255, 255};
                                                SDL_Surface *surf = TTF_RenderText_Solid(font, destinations[i], color);
                                                SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
                                                SDL_Rect pos = {DISPLAY_W / 2 - surf->w / 2, 100 + i * 40, surf->w, surf->h};
                                                SDL_RenderCopy(renderer, tex, NULL, &pos);
                                                SDL_FreeSurface(surf);
                                                SDL_DestroyTexture(tex);
                                            }

                                            SDL_RenderPresent(renderer);

                                            while (SDL_PollEvent(&ev))
                                            {
                                                if (ev.type == SDL_KEYDOWN)
                                                {
                                                    if (ev.key.keysym.sym == SDLK_UP)
                                                        selected = (selected + nb - 1) % nb;
                                                    else if (ev.key.keysym.sym == SDLK_DOWN)
                                                        selected = (selected + 1) % nb;
                                                    else if (ev.key.keysym.sym == SDLK_RETURN)
                                                    {
                                                        const char *map_chemin = NULL;
                                                        int x = 0, y = 0;

                                                        switch (selected)
                                                        {
                                                        case 0: // Maison
                                                            map_chemin = "ressources/maps/Maison_2_Gounix.tmx";
                                                            x = 100;
                                                            y = 420;
                                                            break;
                                                        case 1: // École
                                                            map_chemin = "ressources/maps/Classe_2_Gounix.tmx";
                                                            x = 90;
                                                            y = 80;
                                                            break;
                                                        case 2: // Village
                                                            map_chemin = "ressources/maps/Village_2_Gounix_mini.tmx";
                                                            x = 370;
                                                            y = 330;
                                                            break;
                                                        case 3: // Bar
                                                            map_chemin = "ressources/maps/Bar_2_Gounix.tmx";
                                                            x = 230;
                                                            y = 350;
                                                            break;
                                                        case 4: // Retour
                                                            tp_loop = 0;
                                                            continue;
                                                        }

                                                        if (map_chemin)
                                                        {
                                                            liberer_pnjs(pnjs, nb_pnjs);
                                                            tmx_map_free(*map);
                                                            strcpy(current_map_path, map_chemin);
                                                            *map = tmx_load(current_map_path);
                                                            pnjs = charger_pnjs_depuis_tiled(*map, renderer, &nb_pnjs);
                                                            *current_map_id = get_map_id(current_map_path);
                                                            player->x = x;
                                                            player->y = y;
                                                            tp_loop = 0;

                                                            afficher_dialogue_box("Teleportation effectuee !", renderer, font);
                                                            SDL_RenderPresent(renderer);
                                                            SDL_Delay(1000);
                                                        }
                                                    }
                                                    else if (ev.key.keysym.sym == SDLK_ESCAPE)
                                                    {
                                                        tp_loop = 0;
                                                    }
                                                }
                                            }

                                            SDL_Delay(80);
                                        }
                                    }
                                    break;
                                    case 6:
                                    {
                                        const char *etapes[] = {
                                            "0: Debut - aller voir Mamie",
                                            "1: Trouver le livre de Poincare",
                                            "2: Parler a Kevin et signer",
                                            "3: Aller au bar parler a Roger",
                                            "4: Valider le tampon avec Gaussin",
                                            "5: Obtenir le manuscrit de Klein",
                                            "6: Rejoindre Leserre",
                                        };
                                        for (int i = 0; i < 7; i++)
                                        {
                                            afficher_dialogue_box(etapes[i], renderer, font);
                                            SDL_RenderPresent(renderer);
                                            SDL_Delay(900);
                                        }
                                        afficher_dialogue_box("Choisis une etape (0 a 6)", renderer, font);
                                        SDL_RenderPresent(renderer);
                                        while (SDL_WaitEvent(&ev))
                                        {
                                            if (ev.type == SDL_KEYDOWN &&
                                                ev.key.keysym.sym >= SDLK_0 && ev.key.keysym.sym <= SDLK_6)
                                            {
                                                player->quete.etape = ev.key.keysym.sym - SDLK_0;
                                                break;
                                            }
                                        }
                                        break;
                                    }

                                    case 7:
                                    {
                                        char etat[512];
                                        sprintf(etat,
                                                "PV:%d FAIM:%d ENERG:%d HEURE:%dh\nETAPE:%d\nLIV:%d STY:%d TAM:%d MAN:%d AUT:%d\nGain Nourriture:%d Gain Sommeil:%d",
                                                player->stats.pv, player->stats.faim, player->stats.energie, player->heure_du_jour,
                                                player->quete.etape,
                                                player->inventaire.a_livre, player->inventaire.a_stylo,
                                                player->inventaire.a_tampon, player->inventaire.a_manuscrit,
                                                player->inventaire.a_autorisation,
                                                player->gain_nourriture, player->gain_sommeil);
                                        afficher_dialogue_box(etat, renderer, font);
                                        SDL_RenderPresent(renderer);
                                        SDL_Delay(1500);
                                        break;
                                    }

                                    case 8:
                                        admin_loop = 0;
                                        break;
                                    }
                                }
                                else if (e_admin.key.keysym.sym == SDLK_ESCAPE)
                                {
                                    admin_loop = 0;
                                }
                            }
                        }

                        SDL_Delay(80);
                    }
                    break;
                }

                case SDLK_o:
                {
                    char objectif[512];

                    switch (player->quete.etape)
                    {
                    case 0:
                        strcpy(objectif, "Va parler a Mamie pour debuter ta quete.");
                        break;
                    case 1:
                        strcpy(objectif, "Trouve le livre de Poincare dans la maison.");
                        break;
                    case 2:
                        strcpy(objectif, "Parle a Kevin et donne lui un autographe.");
                        break;
                    case 3:
                        strcpy(objectif, "Va au bar parler a Roger.");
                        break;
                    case 4:
                        strcpy(objectif, "Va voir Gaussin pour valider le tampon.");
                        break;
                    case 5:
                        strcpy(objectif, "Va rencontrer Etienne Klein au bar.");
                        break;
                    case 6:
                        strcpy(objectif, "Va voir Leserre avec tous les objets.");
                        break;
                    case 7:
                        strcpy(objectif, "Objectif final accompli. Tu es le chaos.");
                        break;
                    default:
                        strcpy(objectif, "Objectif inconnu.");
                        break;
                    }

                    afficher_dialogue_box(objectif, renderer, font);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(1500);
                    break;
                }
                case SDLK_ESCAPE:
                {
                    int pause_result = pause_menu(renderer, font);
                    if (pause_result == 1)
                    {
                        if (pause_result == 1)
                        {
                            afficher_dialogue_box("Appuie sur 1 ou 2 pour sauvegarder.", renderer, font);
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
                                        slot = 0;
                                        break;
                                    }
                                }
                            }

                            if (slot != 0)
                            {
                                const char *filename = (slot == 2) ? "save2.dat" : "save1.dat";
                                if (sauvegarder_jeu(player, current_map_path, filename))
                                {
                                    afficher_dialogue_box("Sauvegarde reussie !", renderer, font);
                                }
                                else
                                {
                                    afficher_dialogue_box("Erreur sauvegarde !", renderer, font);
                                }
                            }
                            else
                            {
                                afficher_dialogue_box("Sauvegarde annulee.", renderer, font);
                            }

                            SDL_RenderPresent(renderer);
                            SDL_Delay(1000);
                        }
                    }
                    else if (pause_result == 2)
                    {
                        afficher_dialogue_box("Appuie sur 1 ou 2 pour charger une partie.", renderer, font);
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
                                    slot = 0;
                                    break;
                                }
                            }
                        }

                        if (slot != 0)
                        {
                            const char *filename = (slot == 2) ? "save2.dat" : "save1.dat";

                            if (!charger_jeu(player, current_map_path, filename))
                            {
                                afficher_dialogue_box("Echec du chargement.", renderer, font);
                            }
                            else
                            {
                                // 1. Recharge la MAP
                                tmx_map_free(*map);
                                *map = tmx_load(current_map_path);
                                if (!*map)
                                {
                                    tmx_perror("Erreur chargement map");
                                    exit(1);
                                }

                                // 2. Recharge la texture du joueur
                                reload_player_texture(player, renderer, "ressources/pnj/ginoux1.png");

                                // 3. Recharge les PNJ
                                liberer_pnjs(pnjs, nb_pnjs);
                                nb_pnjs = 0;
                                pnjs = charger_pnjs_depuis_tiled(*map, renderer, &nb_pnjs);

                                // 4. Recharge l'ID de map
                                *current_map_id = get_map_id(current_map_path);

                                afficher_dialogue_box("Partie chargee !", renderer, font);
                            }

                            SDL_RenderPresent(renderer);
                            SDL_Delay(1000);
                        }
                    }
                    else if (pause_result == 3)
                    {
                        run = 0; // Retour menu principal
                    }
                    else if (pause_result == 4)
                    {
                        exit(0); // Quitter jeu
                    }
                    // Si 0, on continue simplement
                    break;
                }

                case SDLK_i:
                    afficher_inventaire(player, renderer, font);
                    break;

                case SDLK_e:
                    for (int i = 0; i < nb_pnjs; i++)
                    {
                        if (pnjs[i] && is_near_pnj(player, pnjs[i], 32.0f))
                        {
                            const char *dialogue = get_dialogue_pnj(player, pnjs[i], renderer, font);
                            afficher_dialogue_box(dialogue, renderer, font);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(3000);
                            pnjs[i]->dernier_dialogue_index = -1;
                            break;
                        }
                    }
                    check_interaction_objets_event(player, *map, pnjs, nb_pnjs, renderer, font);
                    break;

                case SDLK_RETURN:
                    if (dialogue_index != -1 && dialogue_index < nb_pnjs)
                        pnjs[dialogue_index]->dernier_dialogue_index = -1;
                    dialogue_index = -1;
                    break;

                case SDLK_n:
                    player->stats.pv = 0;
                    break;
                }
            }
            if (e.type == SDL_KEYUP)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_w:
                case SDLK_UP:
                    move_y = 0;
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    move_y = 0;
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    move_x = 0;
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
                    move_x = 0;
                    break;
                case SDLK_e:
                    dialogue_index = -1;
                    for (int i = 0; i < nb_pnjs; i++)
                    {
                        if (is_near_pnj(player, pnjs[i], 32))
                        {
                            dialogue_index = i;
                            break;
                        }
                    }

                case SDLK_RETURN:
                    dialogue_index = -1;
                    break;
                }
            }
        }

        update_player_and_handle_exit(player, move_x, move_y, map, renderer, current_map_id, &pnjs, &nb_pnjs);
        update_and_render_stats(player);
        Uint32 now = SDL_GetTicks();
        if (now - player->dernier_tick_heure >= 10000)
        {
            player->heure_du_jour += 1;
            if (player->heure_du_jour >= 24)
                player->heure_du_jour = 0;

            player->dernier_tick_heure = now;
        }

        cam.x = player->x - DISPLAY_W / 2;
        cam.y = player->y - DISPLAY_H / 2;
        SDL_Point cam_offset = {cam.x, cam.y};
        render_map(*map, &cam);
        render_player(player, renderer, &cam);
        for (int i = 0; i < nb_pnjs; i++)
        {
            if (pnjs[i])
                render_pnj(pnjs[i], renderer, &cam);
        }

        render_stats(player);
        if (warningDeath(player, renderer, font))
        {
            run = 0; // Fin de la boucle → retour au menu
            break;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}
