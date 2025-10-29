# Structure du Projet Jean-Mi Simulator

Ce document décrit la nouvelle organisation modulaire du code source du jeu.

## Arborescence

```
tmx/
├── main.c                          # Point d'entrée principal (simplifié)
├── main.c.old                      # Ancien main.c monolithique (sauvegarde)
├── Makefile                        # Makefile pour compilation native
├── Makefile.emscripten             # Makefile pour compilation WebAssembly
├── ressources/                     # Ressources du jeu (sprites, maps, fonts)
└── src/
    ├── tmx.c, tmx_*.c              # Bibliothèque TMX (gestion des cartes Tiled)
    └── game/                       # Modules du jeu
        ├── core/                   # Coeur du jeu
        │   ├── types.h             # Structures de données (Player, PNJ, Stats, etc.)
        │   ├── constants.h         # Constantes du jeu
        │   ├── game_loop.c/h       # Boucle principale du jeu
        │   └── save_load.c/h       # Sauvegarde et chargement
        ├── entities/               # Entités du jeu
        │   ├── player.c/h          # Gestion du joueur
        │   └── pnj.c/h             # Gestion des PNJ et dialogues
        ├── rendering/              # Rendu graphique
        │   ├── renderer.c/h        # Rendu bas niveau (tiles, layers, map)
        │   └── ui.c/h              # Interface utilisateur (menus, dialogues)
        └── world/                  # Monde du jeu
            ├── map.c/h             # Gestion des cartes
            └── interactions.c/h    # Interactions avec le monde
```

## Description des Modules

### Core (Coeur du jeu)

#### types.h
Définit toutes les structures de données utilisées dans le jeu :
- `Quete` : Progression des quêtes
- `Inventaire` : Items possédés par le joueur
- `Stats` : Statistiques (PV, faim, énergie)
- `Player` : Structure complète du joueur
- `PNJ` : Personnages non joueurs
- `Camera` : Gestion de la caméra
- `GameContext` : Contexte global du jeu
- `PNJ_Info` : Configuration des PNJ

#### constants.h
Constantes du jeu :
- `DISPLAY_W`, `DISPLAY_H` : Dimensions de l'écran
- `PLAYER_SPEED`, `PLAYER_SIZE` : Paramètres du joueur
- `INITIAL_ZOOM` : Zoom initial
- `MAX_DIALOGUES` : Nombre maximum de dialogues

#### game_loop.c/h
Boucle principale du jeu :
- `game_loop()` : Gère les événements, le rendu, les interactions
- Menu admin (F9) avec 9 options
- Gestion des objectifs (O)
- Menu pause (Échap)
- Interactions avec les PNJ (E)
- Affichage de l'inventaire (I)

#### save_load.c/h
- `sauvegarder_jeu()` : Sauvegarde l'état du jeu
- `charger_jeu()` : Charge une partie sauvegardée

### Entities (Entités)

#### player.c/h
Gestion du joueur :
- `init_player()` : Initialisation du joueur
- `update_and_render_stats()` : Mise à jour des statistiques
- `reload_player_texture()` : Rechargement de la texture
- `warningDeath()` : Gestion des alertes et de la mort

#### pnj.c/h
Gestion des PNJ :
- Base de données des PNJ (mamie, michel, kevin, klein, roger, gaussin, bourbaki, leserre, fourier)
- `init_pnj()` : Initialisation d'un PNJ
- `init_pnj_from_name()` : Initialisation par nom
- `is_near_pnj()` : Détection de proximité
- `charger_pnjs_depuis_tiled()` : Chargement depuis Tiled
- `liberer_pnjs()` : Libération de mémoire
- `get_dialogue_pnj()` : Récupération des dialogues selon la quête
- `get_random_dialogue()` : Dialogues aléatoires

### Rendering (Rendu)

#### renderer.c/h
Rendu bas niveau :
- `SDL_tex_loader()` : Chargement de textures
- `set_color()` : Configuration de couleur
- `draw_tile()` : Dessin d'une tuile
- `draw_layer()` : Dessin d'une couche
- `draw_all_layers()` : Dessin de toutes les couches
- `render_map()` : Rendu de la carte
- `render_player()` : Rendu du joueur
- `render_pnj()` : Rendu d'un PNJ
- `printf_2D()` : Affichage de texte
- `draw_status_bar_label()` : Barre de statut
- `render_stats()` : Rendu des statistiques

Variables globales :
- `ren` : Renderer SDL
- `global_font` : Police TTF

#### ui.c/h
Interface utilisateur :
- `show_menu()` : Menu principal
- `afficher_dialogue_box()` : Affichage de dialogues
- `split_text()` : Découpage de texte (word wrap)
- `intro_box()` : Cinématique d'introduction
- `pause_menu()` : Menu pause
- `afficher_inventaire()` : Affichage de l'inventaire
- `afficher_cinematique_fin()` : Cinématique de fin
- `mini_quiz_klein()` : Quiz de Klein

### World (Monde)

#### map.c/h
Gestion des cartes :
- `current_map_path` : Chemin de la carte actuelle
- `get_map_id()` : Récupération de l'ID de la carte

#### interactions.c/h
Interactions avec le monde :
- `check_interaction_objets_event()` : Vérification des interactions
- `interaction_nourriture()` : Interaction avec la nourriture
- `interaction_lit()` : Interaction avec le lit (repos)
- `update_player_and_handle_exit()` : Déplacements et transitions

## Compilation

### Compilation Native (Linux/Unix)
```bash
make
make run
```

### Compilation WebAssembly (Emscripten)
```bash
./build_web.sh
# ou
emmake make -f Makefile.emscripten
```

## Avantages de la Structure Modulaire

1. **Lisibilité** : Code organisé par fonctionnalité
2. **Maintenabilité** : Modifications ciblées faciles
3. **Réutilisabilité** : Modules indépendants
4. **Compilation parallèle** : Modules compilés séparément
5. **Débogage** : Erreurs plus faciles à localiser
6. **Collaboration** : Plusieurs développeurs peuvent travailler sur différents modules

## Migration depuis l'Ancien Code

L'ancien fichier monolithique `main.c` (2450 lignes) a été sauvegardé dans `main.c.old`.
Le nouveau `main.c` ne fait que ~200 lignes et sert de point d'entrée qui utilise les modules.

Toutes les fonctionnalités ont été préservées :
- Système de quêtes complet
- Tous les PNJ et leurs dialogues
- Menu admin avec 9 options
- Sauvegarde/chargement
- Interactions avec le monde
- Cinématiques intro/fin
- Quiz Klein

## Notes

- Les includes utilisent des chemins relatifs (`../core/types.h`)
- Les variables globales `ren` et `global_font` sont définies dans `renderer.c`
- Le système de quêtes est géré dans les dialogues PNJ (`pnj.c`)
- La carte actuelle est stockée dans `current_map_path` (`map.c`)
