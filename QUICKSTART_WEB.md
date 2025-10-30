# Quick Start - Version Web

Guide rapide pour mettre votre jeu en ligne en quelques minutes !

## 🚀 Compilation rapide (2 minutes)

### 1. Installer Emscripten

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
cd /home/user/ProjetGounixWEB/tmx
./build_web.sh
```

C'est tout ! Les fichiers sont prêts à être déployés.

---

## 🌐 Déploiement rapide

### Option 1 : GitHub Pages (gratuit, 5 minutes)

**Méthode automatique avec le script :**

```bash
cd /home/user/ProjetGounixWEB

# Créer la branche de déploiement
git checkout -b gh-pages

# Copier les fichiers compilés à la racine
cp tmx/index.html .
cp tmx/jeu.js .
cp tmx/jeu.wasm .
cp tmx/jeu.data .

# Commit et push
git add index.html jeu.js jeu.wasm jeu.data
git commit -m "🎮 Deploy Jean-Mi Simulator web version"
git push -u origin gh-pages
```

**Ensuite sur GitHub :**
1. Allez dans Settings > Pages
2. Source : `gh-pages` branch
3. Cliquez sur Save

Votre jeu sera en ligne à : `https://<votre-username>.github.io/<nom-du-repo>/`

**Pour mettre à jour :**
```bash
git checkout claude/session-011CUZWtH32UyQRCeU9RcE3w  # Retour à la branche de dev
cd tmx
./build_web.sh production  # Recompiler

git checkout gh-pages
cp tmx/index.html tmx/jeu.js tmx/jeu.wasm tmx/jeu.data .
git add .
git commit -m "🔄 Update web version"
git push
```

---

### Option 2 : Netlify (gratuit, 2 minutes)

**Via l'interface web :**
1. Allez sur [netlify.com](https://www.netlify.com)
2. Créez un compte (gratuit)
3. Cliquez sur "Add new site" > "Deploy manually"
4. Glissez-déposez le dossier `tmx/` contenant les fichiers compilés
5. C'est en ligne !

**Via la ligne de commande :**
```bash
npm install -g netlify-cli
cd /home/user/ProjetGounixWEB/tmx
netlify deploy --prod
```

---

### Option 3 : Test local avec Flask (30 secondes)

```bash
cd /home/user/ProjetGounixWEB

# Installer Flask si nécessaire
pip3 install -r requirements.txt

# Démarrer le serveur
./start_server.sh
```

Ouvrez http://localhost:8000 dans votre navigateur.

Le serveur Flask offre de meilleures performances et une configuration optimisée pour WebAssembly.

---

## 📦 Fichiers nécessaires pour le déploiement

Vous devez uploader ces 4 fichiers :

```
✓ index.html    (~10 KB)   - Page web
✓ jeu.js        (~500 KB)  - Code JavaScript
✓ jeu.wasm      (~2 MB)    - Binaire WebAssembly
✓ jeu.data      (~17 MB)   - Assets du jeu
```

**Total : ~20 MB**

---

## 🎮 Tester votre jeu

Contrôles :
- **↑ W A S D** : Déplacements
- **E** : Interagir avec les PNJ/objets
- **I** : Inventaire
- **ESC** : Menu pause
- **F9** : Menu admin (debug)

---

## ⚡ Optimisation (optionnel)

### Réduire la taille

**Compiler en mode production :**
```bash
./build_web.sh production
```

**Optimiser les images :**
```bash
sudo apt install optipng
find ressources -name "*.png" -exec optipng -o7 {} \;
./build_web.sh production
```

Cela peut réduire la taille de `jeu.data` de 30-40%.

---

## 🐛 Problèmes courants

### Le jeu ne charge pas
- **Vérifiez la console** (F12 dans le navigateur)
- **Utilisez un serveur HTTP** (pas file://)
- **Vérifiez que tous les 4 fichiers sont présents**

### "Failed to load jeu.data"
- Le fichier est trop gros pour être chargé
- Vérifiez votre connexion internet
- Attendez quelques secondes de plus

### Le jeu est lent
- **Testez sur Chrome** (meilleures perfs WebAssembly)
- **Compilez en mode production**
- **Fermez les autres onglets**

---

## 📚 Documentation complète

Pour plus de détails, consultez [README_WEB.md](README_WEB.md)

---

**Bon jeu ! 🎮**