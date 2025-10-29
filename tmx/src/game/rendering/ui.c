#include "ui.h"
#include "../core/constants.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SDL2/SDL_image.h>

int show_menu(SDL_Renderer *renderer, TTF_Font *font, Player *joueur_actuel)
{
    const char *options[] = {"Nouvelle Partie", "Charger Partie", "Quitter"};
    const int nb_options = 3;
    int selected = 0;
    SDL_Event e;
    int loop = 1;

    while (loop)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < nb_options; i++)
        {
            SDL_Color color = i == selected ? (SDL_Color){255, 255, 0} : (SDL_Color){255, 255, 255};

            SDL_Surface *text = TTF_RenderText_Solid(font, options[i], color);
            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, text);
            SDL_Rect pos = {DISPLAY_W / 2 - text->w / 2, 200 + i * 60, text->w, text->h};
            SDL_RenderCopy(renderer, tex, NULL, &pos);
            SDL_FreeSurface(text);
            SDL_DestroyTexture(tex);
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                return 2; // Quitter
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    selected = (selected + nb_options - 1) % nb_options;
                    break;
                case SDLK_DOWN:
                    selected = (selected + 1) % nb_options;
                    break;
                case SDLK_RETURN:
                    loop = 0;
                    break;
                case SDLK_ESCAPE:
                    selected = 2; // Quitter
                    loop = 0;
                    break;
                }
            }
        }

        SDL_Delay(100);
    }

    return selected;
}

void afficher_dialogue_box(const char *texte, SDL_Renderer *renderer, TTF_Font *font)
{
    int box_w = 700;
    int box_h = 170;
    int box_x = (DISPLAY_W - box_w) / 2;
    int box_y = DISPLAY_H - box_h - 10;

    SDL_Rect fond = {box_x, box_y, box_w, box_h};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 240);
    SDL_RenderFillRect(renderer, &fond);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &fond);

    SDL_Color noir = {0, 0, 0, 255};

    // Utilisation du word wrap
    char lignes[6][128];
    int nb_lignes = split_text(texte, lignes, 6, 80); // 6 lignes max, 80 caractères max/ligne

    for (int i = 0; i < nb_lignes; i++)
    {
        SDL_Surface *surf = TTF_RenderText_Solid(font, lignes[i], noir);
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
        int text_x = box_x + 20;
        int text_y = box_y + 15 + i * (surf->h + 3);
        SDL_Rect pos = {text_x, text_y, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, NULL, &pos);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }
}

int split_text(const char *texte, char lignes[][128], int max_lignes, int max_chars)
{
    int nb_lignes = 0;
    const char *p = texte;
    while (*p && nb_lignes < max_lignes)
    {
        // Ignore les espaces en debut de ligne
        while (*p && isspace(*p))
            p++;

        int len = 0;
        int last_space = -1;
        while (p[len] && len < max_chars)
        {
            if (isspace(p[len]))
                last_space = len;
            if (p[len] == '\n')
            {
                last_space = len;
                break;
            }
            len++;
        }
        // On evite de couper au milieu d'un mot si possible
        if (p[len] && last_space > 0 && last_space < len)
            len = last_space;
        // Copie la sous-chaîne
        strncpy(lignes[nb_lignes], p, len);
        lignes[nb_lignes][len] = '\0';
        nb_lignes++;
        // Passe à la suite du texte
        p += len;
        while (*p && isspace(*p))
            p++;
    }
    return nb_lignes;
}

void intro_box(SDL_Renderer *renderer, TTF_Font *font)
{
    const char *intro[] = {
        "Jean-Mi vient tout juste de decrocher son diplome de mathematiques.",
        "Fier, il retourne voir son ancien professeur pour lui montrer.",
        "Le professeur n'en croit pas ses yeux. Il est completement choque par ce prodige.",
        "Les annees passent. Jean-Mi, fascine par le chaos, se plonge dans la recherche.",
        "Il etudie la vie, la meteo, les relations humaines, tout semble echapper a l'ordre.",
        "Au fil de ses lectures, il decouvre aussi l'histoire des sciences.",
        "A 25 ans, il devient passionne par Einstein et la theorie de la relativite.",
        "Fier de ses decouvertes, Jean-Mi retourne voir son mentor pour partager ses lectures.",
        "Le vieux professeur lui lance : \n La theorie de la relativite restreinte, c'est Poincare qui l'a trouve, pas Einstein, jeune loup",
        "C'est un choc pour Jean-Mi !",
        "Il quitte la salle, decide a en apprendre plus sur Poincare.",
        "Il se plonge dans les biographies et decouvre une vie incroyable, pleine de panache.",
        "Plus tard, il tente de convaincre ses collegues de la grandeur de Poincare.",
        "Collegue : \nC'est Einstein qui a tout trouve, pas la peine de debattre...",
        "Decourage, Jean-Mi retourne voir son mentor.",
        "Jean-Mi : \nC'est impossible de leur faire entendre raison... Je suis desempare.",
        "Professeur : \nRegle numero 3, la vie est injuste. La majorite prefere la legende a la realite.",
        "Jean-Mi : \nJamais je n'abandonnerai. Je veux redonner a Poincare la place qu'il merite.",
        "Professeur : \nSois pret a tout perdre. Travail, maison, jusqu'a ton ame...",
        "Les paroles du professeur furent prophetiques. Jean-Mi perdit tout.",
        "Objectif : Ecrire des livres pour redorer la gloire de Poincare.",
        "Devenir un specialiste du chaos.",
        "Et pourquoi pas, vivre de sa plume.",
        "Pour l'instant, Jean-Mi est retourne vivre chez sa mere.",
        "Il cherche du travail, et une offre arrive de la NESI, a condition de ne plus parler de Poincare.",
        "Jean Mi :\nTres bien, tant qu'on ne m'empeche pas d'ecrire !\"",
        "Jean Mi ton aventure commence.\nVa parler a Mamie pour debuter ta quete.",
        "Les touches utiles : \n e pour interagir avec les PNJ \n i pour inventaire \n n pour aller niquer ta race \n o pour l'objectif \n",
        NULL};

    int idx = 0;
    SDL_Event e;
    int continuer = 1;
    while (continuer)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (strlen(intro[idx]) > 0)
        {
            afficher_dialogue_box(intro[idx], renderer, font);
        }
        SDL_RenderPresent(renderer);

        int avancer = 0;
        while (!avancer && SDL_WaitEvent(&e))
        {
            if (e.type == SDL_QUIT)
                exit(0);
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
            {
                avancer = 1;
            }
        }
        idx++;
        if (intro[idx] == NULL)
            continuer = 0;
    }
}

int pause_menu(SDL_Renderer *renderer, TTF_Font *font)
{
    const char *options[] = {
        "Reprendre",
        "Sauvegarder la partie",
        "Charger une partie",
        "Retour Menu Principal",
        "Quitter le jeu"};
    const int nb_options = 5;
    int selected = 0;
    SDL_Event e;
    int running = 1;

    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_RenderClear(renderer);

        for (int i = 0; i < nb_options; i++)
        {
            SDL_Color color = (i == selected) ? (SDL_Color){255, 255, 0} : (SDL_Color){255, 255, 255};
            SDL_Surface *text = TTF_RenderText_Solid(font, options[i], color);
            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, text);
            SDL_Rect pos = {DISPLAY_W / 2 - text->w / 2, 200 + i * 50, text->w, text->h};
            SDL_RenderCopy(renderer, tex, NULL, &pos);
            SDL_FreeSurface(text);
            SDL_DestroyTexture(tex);
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                return 3; // Quitter jeu
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    selected = (selected + nb_options - 1) % nb_options;
                    break;
                case SDLK_DOWN:
                    selected = (selected + 1) % nb_options;
                    break;
                case SDLK_RETURN:
                    return selected; // 0 = Reprendre, 1 = Sauvegarder, 2 = Menu principal, 3 = Quitter
                }
            }
        }

        SDL_Delay(100);
    }

    return 0;
}

void afficher_inventaire(Player *p, SDL_Renderer *renderer, TTF_Font *font)
{
    char texte[512];
    sprintf(texte,
            "Inventaire :\n"
            "- Livre : %s\n"
            "- Stylo : %s\n"
            "- Tampon : %s\n"
            "- Manuscrit : %s\n"
            "- Autorisation : %s",
            p->inventaire.a_livre ? "oui" : "non",
            p->inventaire.a_stylo ? "oui" : "non",
            p->inventaire.a_tampon ? "oui" : "non",
            p->inventaire.a_manuscrit ? "oui" : "non",
            p->inventaire.a_autorisation ? "oui" : "non");

    afficher_dialogue_box(texte, renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(1500);
}

int mini_quiz_klein(SDL_Renderer *renderer, TTF_Font *font)
{
    const char *question = "Klein :\nSelon Poincare, qu est ce qui caracterise le chaos ?";
    const char *options[] = {
        "1. Le hasard pur",
        "2. La sensibilite aux conditions initiales",
        "3. L'absence totale de loi"};
    int bonne_reponse = 2; // option 2 (index 1) est correcte

    afficher_dialogue_box(question, renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

    for (int i = 0; i < 3; i++)
    {
        afficher_dialogue_box(options[i], renderer, font);
        SDL_RenderPresent(renderer);
        SDL_Delay(2000);
    }

    afficher_dialogue_box("Klein :\nAppuie sur 1, 2 ou 3 pour repondre...", renderer, font);
    SDL_RenderPresent(renderer);

    SDL_Event e;
    while (SDL_WaitEvent(&e))
    {
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_1:
                return bonne_reponse == 1;
            case SDLK_2:
                return bonne_reponse == 2;
            case SDLK_3:
                return bonne_reponse == 3;
            }
        }
    }

    return 0; // défaut : échoué
}

void afficher_cinematique_fin(SDL_Renderer *renderer, TTF_Font *font, Player *player)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Chargement des images
    SDL_Surface *img_prof = IMG_Load("ressources/sprites/prof.png");
    SDL_Surface *img_poincare = IMG_Load("ressources/sprites/poincare.png");
    SDL_Texture *tex_prof = NULL;
    SDL_Texture *tex_poincare = NULL;

    if (img_prof)
        tex_prof = SDL_CreateTextureFromSurface(renderer, img_prof);
    if (img_poincare)
        tex_poincare = SDL_CreateTextureFromSurface(renderer, img_poincare);

    // Étape 1
    afficher_dialogue_box("La salle est pleine. Un silence absolu regne.", renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_RenderClear(renderer);

    // Étape 2
    afficher_dialogue_box("Jean-Mi entre. Dans sa main, le manuscrit du chaos.", renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_RenderClear(renderer);

    // Étape 3 : mauvaise image
    if (tex_prof)
    {
        SDL_Rect dest = {300, 180, 200, 200};
        SDL_RenderCopy(renderer, tex_prof, NULL, &dest);
    }

    afficher_dialogue_box("Une lumiere se fait. Le portrait de... euh ?", renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);

    // Gag : mauvaise image
    afficher_dialogue_box("Merde... c'est pas la bonne image.", renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_RenderClear(renderer);

    // Étape 4 : bonne image
    if (tex_poincare)
    {
        SDL_Rect dest = {300, 180, 200, 200};
        SDL_RenderCopy(renderer, tex_poincare, NULL, &dest);
    }

    afficher_dialogue_box("Le portrait de Poincarre s'illumine.", renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(2500);
    SDL_RenderClear(renderer);

    // Étape 5
    afficher_dialogue_box("Vous avez rehabilite Poincarre.", renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_RenderClear(renderer);

    // Étape 6
    afficher_dialogue_box("FIN", renderer, font);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);

    // Nettoyage
    if (img_prof)
        SDL_FreeSurface(img_prof);
    if (img_poincare)
        SDL_FreeSurface(img_poincare);
    if (tex_prof)
        SDL_DestroyTexture(tex_prof);
    if (tex_poincare)
        SDL_DestroyTexture(tex_poincare);

    exit(0);
}
