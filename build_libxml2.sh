#!/bin/bash
# Script pour compiler libxml2 pour Emscripten
# Ce script doit être exécuté une seule fois pour préparer la bibliothèque

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
LIBS_DIR="$SCRIPT_DIR/libs"
LIBXML2_DIR="$LIBS_DIR/libxml2"
LIBXML2_BUILD_DIR="$LIBXML2_DIR/build"

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  Compilation de libxml2 pour Emscripten"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""

# Vérifier qu'Emscripten est installé
if ! command -v emcc &> /dev/null; then
    echo "❌ Erreur: Emscripten n'est pas installé ou pas activé"
    echo ""
    echo "Pour activer Emscripten:"
    echo "  source ~/emsdk/emsdk_env.sh"
    exit 1
fi

echo "✓ Emscripten détecté: $(emcc --version | head -n1)"
echo ""

# Si libxml2 existe déjà et est compilé, demander si on recompile
if [ -f "$LIBXML2_BUILD_DIR/libxml2.a" ]; then
    echo "⚠️  libxml2 est déjà compilé"
    echo ""
    read -p "Voulez-vous recompiler? (o/N) " -n 1 -r
    echo ""
    if [[ ! $REPLY =~ ^[Oo]$ ]]; then
        echo "✓ Utilisation de la version existante"
        exit 0
    fi
    rm -rf "$LIBXML2_BUILD_DIR"
fi

# Cloner libxml2 si nécessaire
if [ ! -d "$LIBXML2_DIR" ]; then
    echo "📦 Téléchargement de libxml2..."
    mkdir -p "$LIBS_DIR"
    cd "$LIBS_DIR"
    git clone --depth 1 https://github.com/GNOME/libxml2.git
    echo ""
fi

# Créer le dossier de build
mkdir -p "$LIBXML2_BUILD_DIR"
cd "$LIBXML2_BUILD_DIR"

echo "🔧 Configuration de libxml2..."
emcmake cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DLIBXML2_WITH_PYTHON=OFF \
    -DLIBXML2_WITH_PROGRAMS=OFF \
    -DLIBXML2_WITH_TESTS=OFF \
    -DLIBXML2_WITH_THREADS=OFF \
    -DLIBXML2_WITH_HTTP=OFF

echo ""
echo "🔨 Compilation de libxml2..."
emmake make -j$(nproc)

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "✓ libxml2 compilé avec succès!"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""
echo "Bibliothèque: $LIBXML2_BUILD_DIR/libxml2.a"
echo "Taille: $(du -h "$LIBXML2_BUILD_DIR/libxml2.a" | cut -f1)"
echo ""
echo "Vous pouvez maintenant compiler votre projet avec:"
echo "  cd tmx"
echo "  ./build_web.sh"