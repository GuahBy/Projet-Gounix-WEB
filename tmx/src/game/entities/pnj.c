#include "pnj.h"
#include "../core/constants.h"
#include "../rendering/ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL_image.h>

PNJ_Info pnj_infos[] = {
    {"mamie", "ressources/pnj/mamie.png", {"Mamie : \nBonjour mon petit.", "Mamie : \nVeux-tu un the mathematique ?", "Mamie : \nJe sens que tu es fatigue...", NULL}},
    {"michel", "ressources/pnj/michel.png", {"Michel : \nEncore un bug ?", "Michel : \nC etait mieux avant.", "Michel : \nT as pas honte d'appeler ca du code ?", NULL}},
    {"kevin", "ressources/pnj/kevin.png", {"Kevin : \nJ ai un projet IA en crypto NFT quantique !", "Kevin : \nTu me suis sur Insta ?", "Kevin : \nWesh, moi je code en Rust depuis la maternelle.", NULL}},
    {"klein", "ressources/pnj/klein.png", {"Klein :\nLe temps n existe peut-être pas.", "Klein :\nEt si la carte etait le territoire ?", NULL}},
    {"roger", "ressources/pnj/roger.png", {"Roger :\nC est encore la faute du management !", "Roger :\nLa reforme des outils ? J en veux pas !", NULL}},
    {"gaussin", "ressources/pnj/gaussin.png", {"Gaussin :\nIl manque le formulaire B27.", "Gaussin :\nPour une derogation il te faut 3 tampons.", NULL}},
    {"bourbaki", "ressources/pnj/bourbaki.png", {"Bourbaki : \nOn ne triche pas avec les axiomes.", "Bourbaki : \nLis les elements de mathematique et reviens.", NULL}},
    {"leserre", "ressources/pnj/leserre.png", {"Leserre : \nLa logique precede le verbe.", "Leserre : \nTrouve le paradoxe dans ma quete.", NULL}},
    {"fourier", "ressources/pnj/fourier.png", {"Fourier : \nResous cette FFT pour gagner un bonus.", "Fourier : \nTu veux apprendre l'analyse ?", NULL}}};

const int NB_PNJ_INFOS = sizeof(pnj_infos) / sizeof(PNJ_Info);

PNJ *init_pnj(SDL_Renderer *renderer, const char *sprite_path, int dx, int dy)
{
    PNJ *pnj = malloc(sizeof(PNJ));
    SDL_Surface *surface = IMG_Load(sprite_path);
    if (!surface)
    {
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        free(pnj);
        return NULL;
    }
    pnj->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!pnj->texture)
    {
        fprintf(stderr, "Erreur lors de la creation de la texture : %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(pnj);
        return NULL;
    }
    pnj->x = dx;
    pnj->y = dy;
    pnj->size = PLAYER_SIZE;
    SDL_FreeSurface(surface);
    return pnj;
}

PNJ *init_pnj_from_name(const char *name, int x, int y, SDL_Renderer *renderer)
{
    for (int i = 0; i < NB_PNJ_INFOS; i++)
    {
        if (strcmp(name, pnj_infos[i].name) == 0)
        {
            PNJ *pnj = malloc(sizeof(PNJ));
            if (!pnj)
            {
                fprintf(stderr, "Erreur allocation PNJ %s\n", name);
                return NULL;
            }
            SDL_Surface *surface = IMG_Load(pnj_infos[i].sprite_path);
            if (!surface)
            {
                fprintf(stderr, "Erreur chargement sprite %s : %s\n", pnj_infos[i].sprite_path, IMG_GetError());
                free(pnj);
                return NULL;
            }
            pnj->texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            if (!pnj->texture)
            {
                fprintf(stderr, "Erreur creation texture pour %s\n", name);
                free(pnj);
                return NULL;
            }
            pnj->x = x;
            pnj->y = y;
            pnj->size = PLAYER_SIZE;
            pnj->dernier_dialogue_index = -1;
            strncpy(pnj->name, name, sizeof(pnj->name));
            return pnj;
        }
    }
    fprintf(stderr, "PNJ '%s' non trouve dans pnj_infos[]\n", name);
    return NULL;
}

int is_near_pnj(Player *player, PNJ *pnj, float dist)
{
    int dx = player->x - pnj->x;
    int dy = player->y - pnj->y;
    return (dx * dx + dy * dy) < (dist * dist);
}

PNJ **charger_pnjs_depuis_tiled(tmx_map *map, SDL_Renderer *renderer, int *nb_pnjs)
{
    tmx_layer *layer = map->ly_head;
    while (layer)
    {
        if (layer->type == L_OBJGR && strcmp(layer->name, "PNJ") == 0)
        {
            tmx_object *obj = layer->content.objgr->head;
            int count = 0;
            tmx_object *tmp = obj;
            while (tmp)
            {
                count++;
                tmp = tmp->next;
            }
            PNJ **pnjs = malloc(sizeof(PNJ *) * count);
            if (!pnjs)
            {
                fprintf(stderr, "Erreur allocation tableau PNJ\n");
                *nb_pnjs = 0;
                return NULL;
            }
            int i = 0;
            while (obj)
            {
                if (obj->name)
                {
                    int px = obj->x;
                    int py = obj->y;
                    PNJ *p = init_pnj_from_name(obj->name, px, py, renderer);
                    if (p)
                    {
                        pnjs[i++] = p;
                    }
                    else
                    {
                        fprintf(stderr, "Erreur chargement PNJ %s\n", obj->name);
                    }
                }
                obj = obj->next;
            }
            *nb_pnjs = i;
            return pnjs;
        }
        layer = layer->next;
    }
    fprintf(stderr, "Aucune couche PNJ trouvee dans la map.\n");
    *nb_pnjs = 0;
    return NULL;
}

void liberer_pnjs(PNJ **pnjs, int nb)
{
    if (!pnjs)
        return;
    for (int i = 0; i < nb; i++)
    {
        if (pnjs[i])
        {
            if (pnjs[i]->texture)
            {
                SDL_DestroyTexture(pnjs[i]->texture);
            }
            free(pnjs[i]);
        }
    }
    free(pnjs);
}

const char *get_dialogue_pnj(Player *player, PNJ *pnj, SDL_Renderer *renderer, TTF_Font *font)
{
    if (!pnj || pnj->name[0] == '\0')
        return "???";

    // Mamie Grothendieck
    if (strcmp(pnj->name, "mamie") == 0)
    {
        if (!player->quete.parle_a_mamie && player->quete.etape == 0)
        {
            player->quete.parle_a_mamie = 1;
            player->quete.etape = 1;
            return "Mamie : \nPoincare... il parlait au vent, et le vent lui repondait.\n Regarde donc dans la vieille etagere dans ma chambre.\n Tu trouveras plein de choses sur sa vie";
        }
        else
        {
            return get_random_dialogue(pnj, "mamie");
        }
    }
    // Pnj Kevin
    else if (strcmp(pnj->name, "kevin") == 0)
    {
        // Premier échange : déclenche l'idée du stylo
        if (player->quete.etape == 2 && !player->quete.parle_a_kevin)
        {
            player->quete.parle_a_kevin = 1;
            return "Kevin : \nTu veux briller ? Signe-moi un autographe sur mon tee-shirt !";
        }

        // Monologue de Jean-Mi si pas encore pris le stylo
        if (player->quete.parle_a_kevin && !player->inventaire.a_stylo)
        {
            return "Jean Mi : \nTiens, j avais un stylo sur mon bureau...";
            SDL_Delay(2000);
        }

        // Signature avec le stylo
        if (player->quete.parle_a_kevin && player->inventaire.a_stylo && !player->quete.a_donne_autographe)
        {
            player->quete.a_donne_autographe = 1;
            player->quete.a_tampon = 1;
            player->inventaire.a_tampon = 1; // Ajoute le tampon à l'inventaire
            player->quete.etape = 3;
            afficher_dialogue_box("Kevin : \nWaw, t'es une legende !\n Tiens, garde ce faux tampon dore.", renderer, font);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);
            return "Jean Mi : \nJe devrais aller au bar discuter avec roger";
            SDL_Delay(2000);
        }
        // Cas par défaut (au tout début par erreur)
        else
        {
            return get_random_dialogue(pnj, "kevin");
        }
    }
    // Roger Pietra
    else if (strcmp(pnj->name, "roger") == 0)
    {
        if (player->heure_du_jour >= 22)
        {
            return "Roger : \nIl est tard... repasse demain.";
        }

        if (player->quete.etape == 3 && player->quete.a_tampon && !player->quete.parle_a_roger)
        {
            player->quete.parle_a_roger = 1;
            player->quete.etape = 4;
            afficher_dialogue_box("Jean Mi : Un fan m a donne un tampon hier !\n Je ne vois pas trop a quoi ca sert.", renderer, font);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);
            return "Roger :\nUn client emeche a perdu un tampon dore comme le tien hier..\n.Tu devrais aller voir Gaussin au village";
        }

        if (player->quete.parle_a_roger)
        {
            return "Roger : \nAlors, t as trouver Gaussin ? Il adore l administratif...";
        }
        else
        {
            return get_random_dialogue(pnj, "roger");
        }
    }
    else if (strcmp(pnj->name, "gaussin") == 0)
    {
        if (player->quete.etape == 4 && player->quete.a_tampon && !player->quete.a_autorisation)
        {
            player->quete.a_autorisation = 1;
            player->quete.etape = 5;
            player->inventaire.a_autorisation = 1;
            player->inventaire.a_tampon = 0; // Perd le tampon
            afficher_dialogue_box("Gaussin : \nOh mon tampon qu il est beau \n Merci", renderer, font);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);
            afficher_dialogue_box("Jean MI : \nJe viens te voir pour te demander une autorisation pour organiser une conference a la NESI \n", renderer, font);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);
            afficher_dialogue_box("Gaussin : \nAttends tu as dis conference ? Interessant. \nMais pour légitimer Pointcarre il te faut un manuscrit !\nVa voir un certain Etienne Klein il traine souvent au bar", renderer, font);
            SDL_RenderPresent(renderer);
            SDL_Delay(5000);
            return "Gaussin : \nPour l autorisation Ce tampon suffira... ";
        }

        if (player->quete.a_autorisation)
        {
            return "Gaussin : \nNe perds pas ton autorisation, hein. C'est une version imprimée de la V10-B.";
        }

        else
        {
            return get_random_dialogue(pnj, "gaussin");
        }
    }
    else if (strcmp(pnj->name, "klein") == 0)
    {
        if (player->quete.etape == 5 && !player->inventaire.a_manuscrit)
        {
            afficher_dialogue_box("Klein : \n Si tu veux deranger le cosmos, reponds a ma question.", renderer, font);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);

            if (mini_quiz_klein(renderer, font))
            {
                player->inventaire.a_manuscrit = 1;
                player->quete.etape = 6;
                afficher_dialogue_box("Klein : \n Bien joue. Voici le manuscrit du chaos.", renderer, font);
                SDL_RenderPresent(renderer);
                SDL_Delay(2000);
                return "Jean Mi : Je peux enfin rehabiliter Pointcare et organiser ma conference a la NESI.";
            }
            else
            {
                return "Klein : \nMauvaise reponse. Reviens quand tu seras pret a comprendre le chaos.";
            }
        }

        if (player->inventaire.a_manuscrit)
        {
            return "Klein : \nGarde le precieusement, ce manuscrit. C est une relique.";
        }
        else
        {
            return get_random_dialogue(pnj, "klein");
        }
    }
    // Bourbaki
    else if (strcmp(pnj->name, "bourbaki") == 0)
    {
        if (player->heure_du_jour >= 20)
        {
            return "Bourbaki : \nJ ai des copies a corrigees... reviens demain.";
        }
        else
        {
            return get_random_dialogue(pnj, "bourbaki");
        }
    }
    else if (strcmp(pnj->name, "leserre") == 0)
    {
        if (player->inventaire.a_livre && player->inventaire.a_autorisation && player->inventaire.a_manuscrit)
        {
            player->quete.etape = 7; // Fin débloquée
            afficher_dialogue_box("Leserre : \nTu oses troubler l ordre ? Tres bien... que le chaos commence.", renderer, font);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);

            afficher_cinematique_fin(renderer, font, player);
            return ""; // plus de dialogue après la cinématique
        }
        else
        {
            afficher_dialogue_box("Leserre : \nJe vais t expliquer le traitement du signal...", renderer, font);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);

            player->stats.pv = 0;                                        // Mort brutale
            return "Jean Mi :\nErreur critique d'absorption spectrale."; // rien d'autre à dire
        }
    }

    // Tous les autres
    return get_random_dialogue(pnj, pnj->name);
}

const char *get_random_dialogue(PNJ *pnj, const char *name)
{
    for (int i = 0; i < NB_PNJ_INFOS; i++)
    {
        if (strcmp(pnj_infos[i].name, name) == 0)
        {
            int count = 0;
            while (count < MAX_DIALOGUES && pnj_infos[i].dialogues[count])
                count++;

            if (count == 0)
                return "…";

            if (pnj->dernier_dialogue_index == -1)
            {
                // Tirage random + affectation
                pnj->dernier_dialogue_index = rand() % count;
            }
            return pnj_infos[i].dialogues[pnj->dernier_dialogue_index];
        }
    }
    return "…";
}
