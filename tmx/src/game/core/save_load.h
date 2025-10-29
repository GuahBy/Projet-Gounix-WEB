#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "types.h"

// Fonctions de sauvegarde et chargement
int sauvegarder_jeu(Player *p, const char *map_path, const char *filename);
int charger_jeu(Player *p, char *destination_path, const char *filename);

#endif // SAVE_LOAD_H
