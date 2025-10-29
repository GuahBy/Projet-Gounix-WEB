# Jean-Mi Simulator - Version Web

Ce guide explique comment compiler et déployer la version web du jeu Jean-Mi Simulator en utilisant Emscripten pour transformer le code C/SDL2 en WebAssembly.

## Table des matières

1. [Prérequis](#prérequis)
2. [Installation d'Emscripten](#installation-demscripten)
3. [Compilation du jeu](#compilation-du-jeu)
4. [Test en local](#test-en-local)
5. [Déploiement](#déploiement)
6. [Dépannage](#dépannage)

---

## Prérequis

Avant de commencer, assurez-vous d'avoir :

- **Git** installé
- **Python 3** installé (pour le serveur web de test)
- **Un navigateur web moderne** (Chrome, Firefox, Edge, Safari)
- **Au moins 2 Go d'espace disque libre** (pour Emscripten et les fichiers compilés)

---

## Installation d'Emscripten

Emscripten est le compilateur qui permet de transformer votre code C en WebAssembly.

### Étape 1 : Télécharger Emscripten SDK

```bash
# Créer un dossier pour Emscripten (en dehors du projet)
cd ~
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
```

### Étape 2 : Installer et activer Emscripten

```bash
# Installer la dernière version stable
./emsdk install latest

# Activer la version installée
./emsdk activate latest

# Configurer les variables d'environnement pour la session actuelle
source ./emsdk_env.sh
```

### Étape 3 : Vérifier l'installation

```bash
emcc --version
```

Vous devriez voir quelque chose comme :
```
emcc (Emscripten gcc/clang-like replacement + linker emulating GNU ld) 3.x.x
```

### Configuration permanente (optionnel)

Pour ne pas avoir à exécuter `source ./emsdk_env.sh` à chaque fois, ajoutez cette ligne à votre `~/.bashrc` ou `~/.zshrc` :

```bash
echo 'source "$HOME/emsdk/emsdk_env.sh"' >> ~/.bashrc
source ~/.bashrc
```

---

## Compilation du jeu

### Étape 1 : Se placer dans le dossier du jeu

```bash
cd /home/user/ProjetGounixWEB/tmx
```

### Étape 2 : Compiler avec Emscripten

```bash
# Compilation standard
emmake make -f Makefile.emscripten

# OU pour une compilation optimisée (production)
emmake make -f Makefile.emscripten production

# OU pour une compilation de débogage (avec source maps)
emmake make -f Makefile.emscripten debug
```

### Fichiers générés

Après une compilation réussie, vous aurez :

- **jeu.html** - Page HTML par défaut (vous pouvez utiliser index.html à la place)
- **jeu.js** - Le code JavaScript Emscripten
- **jeu.wasm** - Le binaire WebAssembly
- **jeu.data** - Tous les assets empaquetés (maps, sprites, fonts)

### Taille des fichiers

⚠️ **Attention** : Le fichier `jeu.data` fait environ **17-20 Mo** car il contient tous les sprites et assets du jeu. Le téléchargement initial peut prendre quelques secondes.

---

## Test en local

Vous **ne pouvez pas** simplement ouvrir le fichier HTML dans votre navigateur (à cause des restrictions CORS). Vous devez utiliser un serveur HTTP local.

### Option 1 : Serveur HTTP Python (recommandé)

```bash
# Depuis le dossier tmx/
python3 -m http.server 8000
```

Puis ouvrez votre navigateur à l'adresse : **http://localhost:8000**

### Option 2 : Serveur Emscripten

```bash
# Depuis le dossier tmx/
emrun index.html
```

Cela lancera automatiquement un serveur et ouvrira le jeu dans votre navigateur.

### Option 3 : Serveur Node.js

Si vous avez Node.js installé :

```bash
npx serve .
```

---

## Déploiement

Une fois le jeu compilé, vous pouvez le déployer sur n'importe quel hébergement web statique.

### Fichiers à déployer

Copiez ces fichiers sur votre serveur web :

```
index.html       (ou jeu.html)
jeu.js
jeu.wasm
jeu.data
```

### Plateformes recommandées

#### 1. **GitHub Pages** (gratuit)

```bash
# Créer une branche gh-pages
git checkout -b gh-pages

# Copier les fichiers compilés
cp tmx/index.html tmx/jeu.js tmx/jeu.wasm tmx/jeu.data .

# Commit et push
git add index.html jeu.js jeu.wasm jeu.data
git commit -m "Deploy web version"
git push origin gh-pages
```

Activez GitHub Pages dans les paramètres du repo (Settings > Pages > Source : gh-pages).

Votre jeu sera accessible à : `https://<username>.github.io/<repo-name>/`

#### 2. **Netlify** (gratuit)

1. Créez un compte sur [netlify.com](https://www.netlify.com)
2. Glissez-déposez le dossier contenant les fichiers compilés
3. Votre jeu est en ligne !

#### 3. **Vercel** (gratuit)

```bash
npm i -g vercel
cd tmx
vercel
```

#### 4. **Serveur web classique (Apache, Nginx)**

Copiez simplement les fichiers dans votre dossier `public_html` ou `/var/www/html`.

### Configuration serveur importante

Pour des performances optimales, configurez votre serveur pour :

1. **Compresser les fichiers** (gzip/brotli)
2. **Mettre en cache** les fichiers .wasm et .data
3. **Définir les bons MIME types** :

```apache
# Pour Apache (.htaccess)
AddType application/wasm .wasm
AddType application/octet-stream .data

# Compression
<IfModule mod_deflate.c>
    AddOutputFilterByType DEFLATE application/javascript
    AddOutputFilterByType DEFLATE application/wasm
</IfModule>
```

```nginx
# Pour Nginx
location ~* \.(wasm|data)$ {
    types {
        application/wasm wasm;
        application/octet-stream data;
    }
    gzip on;
    gzip_types application/wasm application/octet-stream;
}
```

---

## Dépannage

### Le jeu ne charge pas

1. **Vérifiez la console du navigateur** (F12)
2. **Vérifiez que vous utilisez un serveur HTTP** (pas file://)
3. **Vérifiez que tous les fichiers sont présents** (jeu.js, jeu.wasm, jeu.data)

### Erreur "memory access out of bounds"

Augmentez la mémoire dans le Makefile :

```makefile
-sINITIAL_MEMORY=512MB \
-sMAXIMUM_MEMORY=1024MB \
```

### Le jeu est lent

1. **Compilez en mode production** pour des optimisations maximales
2. **Vérifiez l'utilisation GPU** dans la console du navigateur
3. **Testez sur un autre navigateur** (Chrome a généralement les meilleures performances WebAssembly)

### Les assets ne chargent pas

Vérifiez que le fichier `jeu.data` est bien téléchargé (onglet Network dans la console).

Si les chemins sont incorrects, modifiez la ligne dans le Makefile :

```makefile
--preload-file ressources@/ressources
```

### Erreur de compilation "library not found"

Assurez-vous que Emscripten est bien activé :

```bash
source ~/emsdk/emsdk_env.sh
emcc --version
```

### Le jeu se bloque en pleine partie

Cela peut être dû à :
- **Un dialogue bloquant** : ASYNCIFY permet de gérer les `SDL_Delay` et boucles while
- **Mémoire insuffisante** : Augmentez `MAXIMUM_MEMORY`

---

## Optimisations avancées

### Réduire la taille du fichier .data

Si les 17 Mo d'assets sont trop lourds, vous pouvez :

1. **Compresser les images PNG** :
```bash
find ressources -name "*.png" -exec optipng -o7 {} \;
```

2. **Charger les assets à la demande** (lazy loading) au lieu de tout précharger

3. **Utiliser un format d'image plus léger** (WebP au lieu de PNG)

### Améliorer les temps de chargement

1. **Activer la compression Brotli** sur votre serveur
2. **Utiliser un CDN** pour distribuer les fichiers
3. **Afficher un écran de chargement** (déjà implémenté dans index.html)

### Compilation avec optimisations maximales

```bash
emmake make -f Makefile.emscripten clean
emmake make -f Makefile.emscripten production
```

Cela active :
- **-O3** : Optimisations maximales du compilateur
- **--closure 1** : Minification du code JavaScript
- **-flto** : Link-Time Optimization

---

## Support et contribution

- **Issues** : Ouvrez une issue sur GitHub
- **Docs Emscripten** : https://emscripten.org/docs/
- **SDL2 + Emscripten** : https://wiki.libsdl.org/SDL2/README/emscripten

---

## Licence

Ce projet utilise :
- **SDL2** (licence zlib)
- **Emscripten** (licence MIT/LLVM)
- **libxml2** (licence MIT)

Bon jeu ! 🎮