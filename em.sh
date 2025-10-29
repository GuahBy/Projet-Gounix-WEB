#!/bin/bash

# Nettoyer le répertoire existant si nécessaire
if [ -d "libxml2" ]; then
    rm -rf libxml2
fi

# Cloner libxml2
git clone https://gitlab.gnome.org/GNOME/libxml2.git
cd libxml2

# Préparer l'autoconf
./autogen.sh

# Configurer pour Emscripten avec les bons paramètres
emconfigure ./configure \
    --prefix=/home/guahby/libxml2_emscripten \
    --build=x86_64-linux-gnu \
    --host=wasm32-unknown-emscripten \
    --disable-shared \
    --enable-static \
    --without-python \
    --without-zlib \
    --disable-dependency-tracking \
    CC="emcc" \
    CXX="em++" \
    CFLAGS="-O2" \
    CXXFLAGS="-O2"

# Compiler
emmake make
emmake make install
