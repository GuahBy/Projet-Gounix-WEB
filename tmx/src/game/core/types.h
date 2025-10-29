#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <tmx.h>

/* Forward declarations */
typedef struct Quete Quete;
typedef struct Inventaire Inventaire;
typedef struct Stats Stats;
typedef struct Player Player;
typedef struct PNJ PNJ;
typedef struct Camera Camera;
typedef struct GameContext GameContext;
typedef struct PNJ_Info PNJ_Info;

/* Structures de quête */
struct Quete
{
    int etape;
    int parle_a_mamie;
    int a_trouve_livre;
    int parle_a_kevin;
    int a_donne_autographe;
    int a_tampon;
    int parle_a_roger;
    int a_manuscrit;
    int a_autorisation;
    int a_declenche_conference;
};

/* Inventaire du joueur */
struct Inventaire
{
    int a_livre;
    int a_stylo;
    int a_tampon;
    int a_manuscrit;
    int a_autorisation;
};

/* Statistiques du joueur */
struct Stats
{
    int faim;
    int energie;
    int pv;
};

/* Structure du joueur */
struct Player
{
    SDL_Texture *texture;
    int x, y;
    int speed;
    int size;
    int a_force_sur_nourriture;
    int faim_warning_affiche;
    int energie_warning_affiche;
    int mort_affichee;
    int dernier_sommeil;
    int heure_du_jour;
    int dernier_tick_heure;
    Stats stats;
    Inventaire inventaire;
    Quete quete;
    int gain_nourriture;
    int gain_sommeil;
};

/* Structure PNJ (Personnage Non Joueur) */
struct PNJ
{
    SDL_Texture *texture;
    int x, y;
    int size;
    char name[32];
    int dernier_dialogue_index;
};

/* Caméra pour le rendu */
struct Camera
{
    float x, y;
    float zoom;
};

/* Contexte global du jeu */
struct GameContext
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    tmx_map *map;
    int is_running;
};

/* Information de configuration PNJ */
struct PNJ_Info
{
    const char *name;
    const char *sprite_path;
    const char *dialogues[5]; /* MAX_DIALOGUES */
};

#endif /* GAME_TYPES_H */
