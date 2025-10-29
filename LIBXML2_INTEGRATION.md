# Intégration de libxml2 avec Emscripten

Ce document explique comment libxml2 a été intégré au projet pour permettre la compilation du jeu vers WebAssembly.

## Problème

Le projet utilise la bibliothèque TMX (Tiled Map XML) qui dépend de libxml2 pour parser les fichiers de cartes TMX. Emscripten ne fournit pas libxml2 en tant que port officiel, il faut donc le compiler manuellement.

## Solution

### 1. Compilation de libxml2 pour WebAssembly

libxml2 a été compilé pour WebAssembly en utilisant Emscripten :

```bash
# Script automatique fourni
./build_libxml2.sh

# Ou manuellement :
cd libs/libxml2/build
emcmake cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DLIBXML2_WITH_PYTHON=OFF \
    -DLIBXML2_WITH_PROGRAMS=OFF \
    -DLIBXML2_WITH_TESTS=OFF \
    -DLIBXML2_WITH_THREADS=OFF \
    -DLIBXML2_WITH_HTTP=OFF
emmake make -j4
```

### 2. Configuration du Makefile

Le `Makefile.emscripten` a été mis à jour pour :

- **Supprimer** le flag `-sUSE_LIBXML2=1` (n'existe plus dans Emscripten moderne)
- **Ajouter** les chemins d'inclusion vers libxml2 :
  ```makefile
  LIBXML2_INCLUDE = -I../libs/libxml2/include -I../libs/libxml2/build
  ```
- **Lier** la bibliothèque statique libxml2.a :
  ```makefile
  LIBXML2_LIB = ../libs/libxml2/build/libxml2.a
  ```

### 3. Structure des fichiers

```
ProjetGounixWEB/
├── libs/
│   └── libxml2/              # Source de libxml2
│       ├── build/            # Fichiers compilés
│       │   └── libxml2.a     # Bibliothèque statique (1.6MB)
│       └── include/          # Headers
│           └── libxml/       # Headers libxml2
├── tmx/
│   ├── src/
│   │   ├── tmx.h            # Utilise libxml2
│   │   ├── tmx_xml.c        # Parser XML avec libxml2
│   │   └── ...
│   ├── Makefile.emscripten  # Configuré pour libxml2
│   └── build_web.sh         # Script de compilation
└── build_libxml2.sh         # Script pour compiler libxml2
```

## Utilisation

### Première fois

1. **Activer Emscripten** :
   ```bash
   source ~/emsdk/emsdk_env.sh
   ```

2. **Compiler libxml2** (une seule fois) :
   ```bash
   ./build_libxml2.sh
   ```

3. **Compiler le jeu** :
   ```bash
   cd tmx
   ./build_web.sh
   ```

### Compilations suivantes

Une fois libxml2 compilé, vous n'avez plus besoin de le recompiler :

```bash
source ~/emsdk/emsdk_env.sh
cd tmx
./build_web.sh
```

## Test

Un test minimal a été créé pour vérifier que libxml2 fonctionne correctement :

```bash
cd tmx
emmake make -f Makefile.test
# Génère: test_tmx.html, test_tmx.js, test_tmx.wasm
```

## Détails techniques

### Options de compilation libxml2

Les options suivantes ont été désactivées pour réduire la taille et simplifier :

- `LIBXML2_WITH_PYTHON=OFF` : Pas de bindings Python
- `LIBXML2_WITH_PROGRAMS=OFF` : Pas d'outils CLI (xmllint, etc.)
- `LIBXML2_WITH_TESTS=OFF` : Pas de tests unitaires
- `LIBXML2_WITH_THREADS=OFF` : Pas de support multi-thread (non nécessaire pour WebAssembly)
- `LIBXML2_WITH_HTTP=OFF` : Pas de support HTTP/FTP

### Taille de la bibliothèque

- **libxml2.a** : ~1.6 MB (statique, non compressée)
- Une fois liée dans le WASM final, la taille augmente d'environ 400-500 KB

### Compatibilité

- ✅ Emscripten 4.0.18 et supérieur
- ✅ libxml2 version master (latest)
- ✅ Compatible avec SDL2, SDL_Image, SDL_TTF

## Problèmes connus

### Erreur: `-sUSE_LIBXML2=1` n'existe pas

**Solution** : Utilisez le Makefile.emscripten fourni qui lie directement libxml2.a

### Erreur lors de la compilation de libxml2

**Solution** : Vérifiez que vous avez bien activé Emscripten avant de lancer le build :
```bash
source ~/emsdk/emsdk_env.sh
```

### Problèmes de réseau avec les ports Emscripten

Si vous rencontrez des erreurs 403 lors du téléchargement de harfbuzz ou d'autres ports, vous pouvez :

1. Utiliser un proxy ou un VPN
2. Pré-télécharger les ports manuellement
3. Désactiver temporairement SDL_TTF si non nécessaire

## Références

- [libxml2 GitHub](https://github.com/GNOME/libxml2)
- [Emscripten Documentation](https://emscripten.org/docs/)
- [Emscripten Ports System](https://github.com/emscripten-core/emscripten/tree/main/tools/ports)
- [TMX C Loader](https://github.com/baylej/tmx)

## Changelog

- **2025-10-29** : Intégration initiale de libxml2 avec Emscripten
  - Compilation de libxml2 2.x pour WebAssembly
  - Configuration du Makefile.emscripten
  - Création du script build_libxml2.sh
  - Tests de compilation réussis