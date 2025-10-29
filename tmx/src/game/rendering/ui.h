#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../core/types.h"

// Fonctions d'interface utilisateur
int show_menu(SDL_Renderer *renderer, TTF_Font *font, Player *joueur_actuel);
void afficher_dialogue_box(const char *texte, SDL_Renderer *renderer, TTF_Font *font);
int split_text(const char *texte, char lignes[][128], int max_lignes, int max_chars);
void intro_box(SDL_Renderer *renderer, TTF_Font *font);
int pause_menu(SDL_Renderer *renderer, TTF_Font *font);
void afficher_inventaire(Player *p, SDL_Renderer *renderer, TTF_Font *font);
void afficher_cinematique_fin(SDL_Renderer *renderer, TTF_Font *font, Player *player);
int mini_quiz_klein(SDL_Renderer *renderer, TTF_Font *font);

#endif // UI_H
