#!/bin/bash

# Script de compilation et lancement pour la version modulaire
# Utilise gcc directement sans Make

cd "$(dirname "$0")/tmx" || exit 1

echo "🔨 Compilation du jeu (version modulaire)..."

# Compiler tous les modules
gcc -o jeu \
    main.c \
    src/game/core/game_loop.c \
    src/game/core/save_load.c \
    src/game/entities/player.c \
    src/game/entities/pnj.c \
    src/game/rendering/renderer.c \
    src/game/rendering/ui.c \
    src/game/world/map.c \
    src/game/world/interactions.c \
    src/tmx.c \
    src/tmx_xml.c \
    src/tmx_utils.c \
    src/tmx_err.c \
    src/tmx_mem.c \
    src/tmx_hash.c \
    -I./src \
    -I./src/game \
    -lSDL2 -lSDL2_image -lSDL2_ttf -lxml2 -lz -lm

if [ $? -ne 0 ]; then
    echo "❌ Échec de la compilation"
    exit 1
fi

echo "✅ Compilation réussie !"
echo "🚀 Lancement du jeu..."

./jeu