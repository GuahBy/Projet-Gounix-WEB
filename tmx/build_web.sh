#!/bin/bash

# Script de compilation automatique pour Jean-Mi Simulator Web
# Usage: ./build_web.sh [clean|debug|production]

set -e  # Arrêter en cas d'erreur

# Couleurs pour le terminal
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}  Jean-Mi Simulator - Compilation Web${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

# Vérifier qu'Emscripten est installé
if ! command -v emcc &> /dev/null; then
    echo -e "${RED}❌ Erreur: Emscripten n'est pas installé ou pas activé${NC}"
    echo ""
    echo "Pour installer Emscripten:"
    echo "  cd ~"
    echo "  git clone https://github.com/emscripten-core/emsdk.git"
    echo "  cd emsdk"
    echo "  ./emsdk install latest"
    echo "  ./emsdk activate latest"
    echo "  source ./emsdk_env.sh"
    echo ""
    echo "Ou si déjà installé, activez-le:"
    echo "  source ~/emsdk/emsdk_env.sh"
    exit 1
fi

echo -e "${GREEN}✓ Emscripten détecté: $(emcc --version | head -n1)${NC}"
echo ""

# Déterminer le mode de compilation
MODE="all"
if [ "$1" == "clean" ]; then
    MODE="clean"
elif [ "$1" == "debug" ]; then
    MODE="debug"
elif [ "$1" == "production" ] || [ "$1" == "prod" ]; then
    MODE="production"
fi

# Nettoyage
if [ "$MODE" == "clean" ]; then
    echo -e "${YELLOW}🧹 Nettoyage des fichiers de compilation...${NC}"
    emmake make -f Makefile.emscripten clean
    echo -e "${GREEN}✓ Nettoyage terminé${NC}"
    exit 0
fi

# Vérifier que le dossier ressources existe
if [ ! -d "ressources" ]; then
    echo -e "${RED}❌ Erreur: Le dossier 'ressources' n'existe pas${NC}"
    echo "Assurez-vous d'être dans le dossier 'tmx'"
    exit 1
fi

# Afficher la taille des ressources
RESSOURCES_SIZE=$(du -sh ressources | cut -f1)
echo -e "${BLUE}📦 Taille des ressources: ${RESSOURCES_SIZE}${NC}"
echo ""

# Compilation
echo -e "${YELLOW}🔨 Compilation en mode: ${MODE}${NC}"
echo ""

START_TIME=$(date +%s)

if [ "$MODE" == "debug" ]; then
    echo -e "${YELLOW}Mode debug: compilation avec source maps et assertions${NC}"
    emmake make -f Makefile.emscripten debug
elif [ "$MODE" == "production" ]; then
    echo -e "${YELLOW}Mode production: optimisations maximales${NC}"
    emmake make -f Makefile.emscripten production
else
    echo -e "${YELLOW}Mode standard: compilation normale${NC}"
    emmake make -f Makefile.emscripten
fi

END_TIME=$(date +%s)
DURATION=$((END_TIME - START_TIME))

echo ""
echo -e "${GREEN}✓ Compilation réussie en ${DURATION}s${NC}"
echo ""

# Vérifier les fichiers générés
echo -e "${BLUE}📋 Fichiers générés:${NC}"
if [ -f "jeu.html" ]; then
    SIZE=$(du -h jeu.html | cut -f1)
    echo -e "  ${GREEN}✓${NC} jeu.html (${SIZE})"
fi
if [ -f "jeu.js" ]; then
    SIZE=$(du -h jeu.js | cut -f1)
    echo -e "  ${GREEN}✓${NC} jeu.js (${SIZE})"
fi
if [ -f "jeu.wasm" ]; then
    SIZE=$(du -h jeu.wasm | cut -f1)
    echo -e "  ${GREEN}✓${NC} jeu.wasm (${SIZE})"
fi
if [ -f "jeu.data" ]; then
    SIZE=$(du -h jeu.data | cut -f1)
    echo -e "  ${GREEN}✓${NC} jeu.data (${SIZE})"
fi
if [ -f "index.html" ]; then
    SIZE=$(du -h index.html | cut -f1)
    echo -e "  ${GREEN}✓${NC} index.html (${SIZE})"
fi

echo ""

# Taille totale
TOTAL_SIZE=$(du -sh jeu.js jeu.wasm jeu.data 2>/dev/null | awk '{sum+=$1} END {print sum}')
echo -e "${BLUE}📊 Taille totale à déployer: ~$(du -ch jeu.js jeu.wasm jeu.data index.html 2>/dev/null | tail -1 | cut -f1)${NC}"
echo ""

# Instructions pour tester
echo -e "${GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${GREEN}  Pour tester le jeu:${NC}"
echo -e "${GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""
echo "Option 1 (recommandé):"
echo -e "  ${YELLOW}python3 -m http.server 8000${NC}"
echo "  Puis ouvrez: http://localhost:8000"
echo ""
echo "Option 2:"
echo -e "  ${YELLOW}emrun index.html${NC}"
echo ""
echo -e "${BLUE}Fichiers à déployer sur un serveur web:${NC}"
echo "  - index.html"
echo "  - jeu.js"
echo "  - jeu.wasm"
echo "  - jeu.data"
echo ""
echo -e "${GREEN}✨ Compilation terminée avec succès!${NC}"