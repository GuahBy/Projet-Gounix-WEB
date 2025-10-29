#!/bin/bash

# Script de déploiement automatique sur GitHub Pages
# Usage: ./deploy_github_pages.sh

set -e  # Arrêter en cas d'erreur

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}  Déploiement GitHub Pages${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

# Sauvegarder la branche actuelle
CURRENT_BRANCH=$(git branch --show-current)
echo -e "${BLUE}📍 Branche actuelle: ${CURRENT_BRANCH}${NC}"
echo ""

# Vérifier qu'on est dans le bon dossier
if [ ! -d "tmx" ]; then
    echo -e "${RED}❌ Erreur: Vous devez être à la racine du projet${NC}"
    exit 1
fi

# Vérifier qu'Emscripten est disponible
if ! command -v emcc &> /dev/null; then
    echo -e "${RED}❌ Erreur: Emscripten n'est pas activé${NC}"
    echo "Exécutez: source ~/emsdk/emsdk_env.sh"
    exit 1
fi

# Question de confirmation
echo -e "${YELLOW}Ce script va:${NC}"
echo "  1. Compiler le jeu en mode production"
echo "  2. Créer/mettre à jour la branche gh-pages"
echo "  3. Pousser vers GitHub"
echo ""
read -p "Continuer ? (y/n) " -n 1 -r
echo ""

if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo -e "${YELLOW}Annulé.${NC}"
    exit 0
fi

echo ""
echo -e "${YELLOW}🔨 Étape 1/4: Compilation en mode production${NC}"
cd tmx
./build_web.sh production
cd ..
echo -e "${GREEN}✓ Compilation terminée${NC}"
echo ""

echo -e "${YELLOW}🌿 Étape 2/4: Préparation de la branche gh-pages${NC}"

# Créer/checkout la branche gh-pages
if git show-ref --verify --quiet refs/heads/gh-pages; then
    echo "La branche gh-pages existe déjà"
    git checkout gh-pages
else
    echo "Création de la branche gh-pages"
    git checkout -b gh-pages
fi
echo -e "${GREEN}✓ Branche gh-pages prête${NC}"
echo ""

echo -e "${YELLOW}📦 Étape 3/4: Copie des fichiers compilés${NC}"

# Copier les fichiers compilés
cp tmx/index.html .
cp tmx/jeu.js .
cp tmx/jeu.wasm .
cp tmx/jeu.data .

echo -e "${GREEN}✓ Fichiers copiés:${NC}"
ls -lh index.html jeu.js jeu.wasm jeu.data | awk '{print "  " $9 " (" $5 ")"}'
echo ""

echo -e "${YELLOW}📤 Étape 4/4: Commit et push vers GitHub${NC}"

# Commit
git add index.html jeu.js jeu.wasm jeu.data

if git diff --cached --quiet; then
    echo -e "${YELLOW}⚠ Aucun changement détecté${NC}"
else
    COMMIT_MSG="🎮 Update Jean-Mi Simulator web version ($(date '+%Y-%m-%d %H:%M'))"
    git commit -m "$COMMIT_MSG"
    echo -e "${GREEN}✓ Commit créé${NC}"
fi

# Push
echo ""
echo -e "${YELLOW}Pushing vers GitHub...${NC}"
git push -u origin gh-pages

echo ""
echo -e "${GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${GREEN}  ✨ Déploiement terminé avec succès !${NC}"
echo -e "${GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

# Obtenir le nom du repo pour construire l'URL
REPO_URL=$(git config --get remote.origin.url)
if [[ $REPO_URL =~ github.com[:/]([^/]+)/([^/.]+) ]]; then
    USERNAME="${BASH_REMATCH[1]}"
    REPONAME="${BASH_REMATCH[2]}"

    echo -e "${BLUE}🌐 Votre jeu sera bientôt accessible à:${NC}"
    echo ""
    echo -e "  ${GREEN}https://${USERNAME}.github.io/${REPONAME}/${NC}"
    echo ""
    echo -e "${YELLOW}Note: Le déploiement peut prendre 1-2 minutes${NC}"
else
    echo -e "${BLUE}🌐 Votre jeu sera bientôt accessible sur GitHub Pages${NC}"
fi

echo ""
echo -e "${BLUE}📋 Prochaines étapes:${NC}"
echo "  1. Allez dans Settings > Pages de votre repo GitHub"
echo "  2. Vérifiez que la source est 'gh-pages' branch"
echo "  3. Attendez quelques minutes pour le déploiement"
echo ""

# Retourner à la branche d'origine
echo -e "${YELLOW}🔄 Retour à la branche ${CURRENT_BRANCH}${NC}"
git checkout "$CURRENT_BRANCH"

echo -e "${GREEN}✓ Terminé !${NC}"