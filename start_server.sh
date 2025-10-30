#!/bin/bash

echo "=========================================="
echo "  Jean-Mi Gounix Simulator"
echo "  Démarrage du serveur Flask"
echo "=========================================="
echo ""

# Vérifier que Python 3 est installé
if ! command -v python3 &> /dev/null; then
    echo "ERREUR: Python 3 n'est pas installé!"
    echo "Installez Python 3 avec: sudo apt install python3 python3-pip"
    exit 1
fi

# Vérifier que pip est installé
if ! command -v pip3 &> /dev/null; then
    echo "ERREUR: pip3 n'est pas installé!"
    echo "Installez pip3 avec: sudo apt install python3-pip"
    exit 1
fi

# Vérifier si Flask est installé
if ! python3 -c "import flask" 2>/dev/null; then
    echo "Flask n'est pas installé. Installation en cours..."
    pip3 install -r requirements.txt
    echo ""
fi

# Vérifier que le dossier tmx existe
if [ ! -d "tmx" ]; then
    echo "ERREUR: Le dossier 'tmx' n'existe pas!"
    exit 1
fi

# Vérifier que les fichiers du jeu sont compilés
if [ ! -f "tmx/index.html" ] || [ ! -f "tmx/jeu.js" ]; then
    echo "AVERTISSEMENT: Le jeu ne semble pas être compilé!"
    echo "Voulez-vous compiler le jeu maintenant? (o/n)"
    read -r response
    if [ "$response" = "o" ] || [ "$response" = "O" ]; then
        cd tmx || exit 1
        if [ -f "build_web.sh" ]; then
            ./build_web.sh
        else
            echo "ERREUR: build_web.sh introuvable!"
            exit 1
        fi
        cd ..
    else
        echo "Le serveur démarrera sans fichiers compilés."
    fi
fi

echo ""
echo "Démarrage du serveur Flask sur http://localhost:8000 ..."
echo "Appuyez sur Ctrl+C pour arrêter le serveur"
echo ""

# Lancer le serveur Flask
python3 app.py
