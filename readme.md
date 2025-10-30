# 🕹️ Documentation d'Installation et de Lancement du Jeu

---

## 🔧 Prérequis

- Un système d'exploitation **Linux** (Ubuntu, Debian, etc.)
- Une **connexion internet**
- Un **terminal** (raccourci : `Ctrl + Alt + T`)

---

## 📦 Installation du Jeu

### 🅰️ Option 1 : Cloner le dépôt GitHub

```bash
git clone https://github.com/GuahBy/Projet-Gounix-WEB
```

### 🅱️ Option 2 : Télécharger l’archive `.zip`

1. Téléchargez l’archive du jeu.
2. Extrayez-la avec la commande suivante :

```bash
unzip Projet_Gounix.zip
```

---

### 📂 Se rendre dans le dossier du jeu

```bash
cd Projet_Gounix
```

---

### 🔑 Rendre le script exécutable

```bash
chmod +x installation.sh
```

> 🔍 Cela permet au fichier d’être lancé comme un programme.

---

### 🚀 Lancer l’installation

```bash
sudo ./installation.sh
```

> 💡 Le mot de passe administrateur sera demandé pour autoriser l'installation.

Le script :
- Installe les **dépendances nécessaires** (ex : SDL2, SDL_ttf…)
- Configure automatiquement le projet

---

## ▶️ Lancer le Jeu en local 

### 1. Se rendre dans le dossier du jeu exécuté

```bash
cd tmx
```

### 2. Rendre le script de lancement exécutable

```bash
make
```

### 3. Démarrer le jeu

```bash
./jeu
```

## ▶️ Lancer le Jeu en serveur Web

### 1. Installer Emscripten

```bash
cd
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk/
git pull
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```
### 2. Compiler avec Emscripten

```bash
source ./emsdk_env.sh
./build_libxmlé.sh
cd tmx
./build_web.sh
```

### 3. Lancer le serveur

```bash
python3 -m http.server 8000
```
### 4. Jouer depuis le navigateur

```bash
http://localhost:8000
```

---

## ❓ Problèmes Courants

### 🔒 Erreur : `Permission denied`

- Vérifiez que vous avez bien utilisé `chmod +x` sur le script
- Utilisez `sudo` si nécessaire

### 🌐 Échec de téléchargement ou de dépendance

- Vérifiez votre connexion internet
- Relancez `sudo ./installation.sh`

### 🐛 Bug dans le jeu

- Vérifiez les **mises à jour** sur GitHub
- Contactez-nous via l’email indiqué dans le dépôt

---

## 🎮 Contrôles du Jeu

### 🕹️ Navigation et interactions

| Touche(s)           | Action                              |
|---------------------|-------------------------------------|
| `↑ ↓ → ←` ou `ZQSD` | Déplacement du personnage           |
| `E`                 | Interagir avec un PNJ ou un objet   |
| `I`                 | Ouvrir/fermer l'inventaire          |
| `N`                 | [Option humoristique]               |
| `O`                 | Afficher l’objectif de la quête     |

---

## 🧭 Menu ADMIN (Touche `F9`)

> ⚠️ Réservé aux développeurs / debug / triche

### 📋 Navigation

- `↑ ↓` : Naviguer entre les options
- `Entrée` : Valider
- `Échap` ou option 9 : Quitter

### 📜 Fonctions disponibles

| N° | Fonction                  | Description                                            |
|----|---------------------------|--------------------------------------------------------|
| 1  | Modifier les statistiques | Ajuste PV, Faim, Énergie à une valeur entre 10 et 90   |
| 2  | Gains nutrition/sommeil   | Modifie les gains obtenus en mangeant ou dormant       |
| 3  | Gestion du temps          | Avancer ou reculer l'heure simulée                     |
| 4  | Taille des PNJ            | Redimensionner tous les PNJ à une valeur donnée        |
| 5  | Gestion inventaire        | Ajouter, retirer ou inverser les objets                |
| 6  | Téléportation             | Aller directement vers Maison, École, Bar ou Village   |
| 7  | Étape de quête            | Forcer l’étape actuelle de la quête (0 à 6)            |
| 8  | Voir état actuel          | Affiche toutes les stats et l’état du joueur           |
| 9  | Quitter le mode admin     | Retour au jeu normal                                   |

---

## 🎉 C’est terminé !

Vous pouvez maintenant jouer à **Jean-Mi Gounix Simulator** librement 🎉  
Répétez simplement la section "Lancer le Jeu" pour rejouer à tout moment.
