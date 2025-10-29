#!/bin/bash

# 1. Définir les chemins
PROJET_DIR="$PWD"
LIBXML2_DIR="$PROJET_DIR/libxml2"
TMX_DIR="$PROJET_DIR/tmx"

# 2. Compilation de libxml2
echo "Compilation de libxml2..."
cd "$LIBXML2_DIR" || { echo "Échec de l'accès au répertoire libxml2"; exit 1; }
emconfigure ./configure --host=wasm32-emscripten --prefix=/usr/local/emscripten/system --disable-shared --enable-static
emmake make
emmake make install

# 3. Compilation du jeu
echo "Compilation du projet..."
cd "$TMX_DIR" || { echo "Échec de l'accès au répertoire tmx"; exit 1; }
emcc main.c -o output.html -I"$LIBXML2_DIR/include" -I./src -L"$LIBXML2_DIR" -lxml2 -s LLD_REPORT_UNDEFINED

