#ifndef MAP_H
#define MAP_H

// Variable globale pour le chemin de la carte actuelle
extern char current_map_path[128];

// Fonction de gestion de carte
int get_map_id(const char *path);

#endif // MAP_H
