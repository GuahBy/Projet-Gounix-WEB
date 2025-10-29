#!/bin/bash

# 1. Définir les chemins
PROJET_DIR="$PWD"
TMX_DIR="$PROJET_DIR/tmx"
DEFAULT_MAP="Maison_2_Gounix.tmx"

# 2. Vérifier que le dossier tmx existe
if [ ! -d "$TMX_DIR" ]; then
    echo "❌ Le répertoire du projet tmx n'existe pas : $TMX_DIR"
    echo "📌 Assurez-vous que le projet est bien cloné dans : $PROJET_DIR"
    exit 1
fi

# 3. Compilation du jeu
echo "🔨 Compilation du projet..."
cd "$TMX_DIR" || { echo "❌ Échec de l'accès au répertoire tmx"; exit 1; }
gcc -o jeu main.c -I./src -L. -ltmx -lSDL2 -lSDL2_image -lz -lxml2 -lSDL2_ttf -lm

# 4. Vérifier la présence de la carte par défaut
MAP_PATH="ressources/maps/$DEFAULT_MAP"
if [ ! -f "$MAP_PATH" ]; then
    echo "❌ La carte par défaut '$MAP_PATH' est introuvable dans le répertoire courant : $(pwd)"
    echo "📌 Vérifiez que la carte existe dans : $TMX_DIR/ressources/maps/"
    exit 1
fi

# 5. Lancement du jeu avec la carte par défaut
echo "🚀 Lancement du jeu avec la carte par défaut : $MAP_PATH"
if [ ! -f jeu ]; then
    echo "❌ Le binaire 'jeu' n'a pas été trouvé. Compilation échouée ?"
    exit 1
fi
./jeu "$MAP_PATH"
