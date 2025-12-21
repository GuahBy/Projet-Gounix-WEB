#!/bin/bash

# 1. Mise à jour des paquets
echo "🔄 Mise à jour du système..."
sudo apt update -y && sudo apt upgrade -y

# 2. Installation des dépendances
echo "📦 Installation des dépendances..."
sudo apt install -y gcc build-essential cmake git \
    libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev \
    libxml2-dev zlib1g-dev
echo "✔ Installation des réussie ✔"
