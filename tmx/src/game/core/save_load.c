#include "save_load.h"
#include <stdio.h>
#include <string.h>

int sauvegarder_jeu(Player *p, const char *map_path, const char *filename)
{
    FILE *f = fopen(filename, "wb");
    if (!f)
    {
        printf("Erreur ouverture fichier %s\n", filename);
        return 0;
    }

    // Copier le joueur sans la texture
    Player copy = *p;
    copy.texture = NULL;

    // Sauvegarde
    fwrite(&copy, sizeof(Player), 1, f);
    fwrite(map_path, sizeof(char), 128, f);
    fclose(f);
    return 1;
}

int charger_jeu(Player *p, char *destination_path, const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if (!f)
    {
        printf("Erreur ouverture fichier %s\n", filename);
        return 0;
    }

    Player temp;
    fread(&temp, sizeof(Player), 1, f);

    char map_path[128];
    fread(map_path, 128, 1, f);

    fclose(f);

    // Recopie les données
    *p = temp;
    strcpy(destination_path, map_path);

    // On s'assure que la texture sera rechargée proprement
    p->texture = NULL;

    return 1;
}
