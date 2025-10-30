# 🎮 Jean-Mi Simulator - Version Web

## 🎯 Résumé

Ce projet a été adapté pour fonctionner directement dans un navigateur web grâce à **Emscripten**, qui compile le code C/SDL2 en WebAssembly.

## 📁 Nouveaux fichiers ajoutés

### Configuration et build
- **tmx/Makefile.emscripten** - Configuration de compilation Emscripten
- **tmx/build_web.sh** - Script automatique de compilation
- **tmx/index.html** - Page web principale pour héberger le jeu

### Documentation
- **README_WEB.md** - Guide complet et détaillé
- **QUICKSTART_WEB.md** - Guide rapide pour démarrer en 5 minutes
- **WEB_VERSION.md** - Ce fichier (résumé)

### Déploiement
- **deploy_github_pages.sh** - Script de déploiement automatique sur GitHub Pages

### Modifications du code
- **tmx/main.c** - Ajout du support Emscripten avec `#ifdef __EMSCRIPTEN__`

## 🚀 Utilisation rapide

### 1. Installer Emscripten (une seule fois)

```bash
cd ~
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

### 2. Compiler le jeu

```bash
cd ProjetGounixWEB/tmx
./build_web.sh
```

### 3. Tester localement

```bash
# Depuis la racine du projet
./start_server.sh
# Ouvrir http://localhost:8000
```

Le serveur Flask offre de meilleures performances et une configuration optimisée pour WebAssembly.

### 4. Déployer sur GitHub Pages

```bash
cd ProjetGounixWEB
./deploy_github_pages.sh
```

## 🎨 Fonctionnalités

### ✅ Ce qui fonctionne
- ✓ Rendu SDL2 complet
- ✓ Chargement des maps TMX
- ✓ Tous les sprites et assets
- ✓ Système de dialogue
- ✓ Système d'inventaire
- ✓ Système de quêtes
- ✓ NPCs et interactions
- ✓ Menu pause / admin
- ✓ Sauvegarde/chargement (localStorage)

### ⚙️ Technologies utilisées
- **Emscripten** - Compilateur C vers WebAssembly
- **WebAssembly (WASM)** - Exécution haute performance
- **SDL2** - Portée vers le web par Emscripten
- **ASYNCIFY** - Permet les boucles while et SDL_Delay dans le navigateur

## 📊 Spécifications techniques

### Taille des fichiers
- `jeu.js` : ~500 KB (code JavaScript)
- `jeu.wasm` : ~2 MB (binaire WebAssembly)
- `jeu.data` : ~17 MB (assets: maps, sprites, fonts)
- **Total : ~20 MB**

### Temps de compilation
- Mode debug : ~30-60 secondes
- Mode standard : ~40-80 secondes
- Mode production : ~60-120 secondes

### Compatibilité navigateurs
- ✅ Chrome 57+ (recommandé)
- ✅ Firefox 52+
- ✅ Safari 11+
- ✅ Edge 79+

### Performance
- **FPS cible** : 60 FPS
- **Résolution** : 800x600 pixels
- **Mémoire** : ~256 MB (peut aller jusqu'à 512 MB)

## 🔧 Options de compilation

### Standard
```bash
./build_web.sh
```
Compilation normale avec optimisations -O2.

### Debug
```bash
./build_web.sh debug
```
Inclut les source maps et assertions pour le débogage.

### Production
```bash
./build_web.sh production
```
Optimisations maximales (-O3, closure, LTO).

### Nettoyage
```bash
./build_web.sh clean
```
Supprime tous les fichiers compilés.

## 📦 Structure des fichiers web

```
tmx/
├── index.html          # Page principale (custom)
├── jeu.html            # Page générée par Emscripten (optionnel)
├── jeu.js              # Code JavaScript Emscripten
├── jeu.wasm            # Binaire WebAssembly
├── jeu.data            # Assets empaquetés
├── Makefile.emscripten # Configuration de build
└── build_web.sh        # Script de compilation
```

## 🌐 Options de déploiement

### GitHub Pages
```bash
./deploy_github_pages.sh
```
URL : `https://<username>.github.io/<repo>/`

### Netlify
```bash
npm install -g netlify-cli
cd tmx
netlify deploy --prod
```

### Vercel
```bash
npm install -g vercel
cd tmx
vercel
```

### Serveur web personnel
Copiez les 4 fichiers (index.html, jeu.js, jeu.wasm, jeu.data) sur votre serveur.

## 🐛 Dépannage

### Problème : "Emscripten not found"
```bash
source ~/emsdk/emsdk_env.sh
```

### Problème : "Failed to load jeu.data"
- Vérifiez que vous utilisez un serveur HTTP (pas file://)
- Le fichier fait 17 MB, la connexion peut prendre du temps

### Problème : Le jeu est lent
- Compilez en mode production : `./build_web.sh production`
- Testez sur Chrome (meilleures performances WebAssembly)

### Problème : Compilation échoue
```bash
./build_web.sh clean
./build_web.sh
```

## 📚 Ressources

- **Documentation Emscripten** : https://emscripten.org/docs/
- **SDL2 + Emscripten** : https://wiki.libsdl.org/SDL2/README/emscripten
- **WebAssembly** : https://webassembly.org/

## 🎯 Prochaines améliorations possibles

- [ ] Lazy loading des assets (chargement à la demande)
- [ ] Support mobile/tactile
- [ ] Compression des sprites (PNG → WebP)
- [ ] Mode plein écran
- [ ] Multijoueur en ligne (WebRTC/WebSocket)
- [ ] Achievements (stockés dans localStorage)

## 📝 Licence

Ce projet utilise :
- SDL2 (licence zlib)
- Emscripten (licence MIT/LLVM)
- libxml2 (licence MIT)

---

**Créé avec ❤️ par l'équipe Gounix**

Pour toute question, consultez :
- [README_WEB.md](README_WEB.md) - Documentation complète
- [QUICKSTART_WEB.md](QUICKSTART_WEB.md) - Guide rapide