#!/usr/bin/env python3
"""
Serveur Flask pour Jean-Mi Gounix Simulator (Version Web)
Sert les fichiers statiques du jeu compilé avec Emscripten
"""

from flask import Flask, send_from_directory, send_file
import os

app = Flask(__name__)

# Dossier contenant les fichiers du jeu compilé
GAME_DIR = os.path.join(os.path.dirname(__file__), 'tmx')

@app.route('/')
def index():
    """Sert la page principale du jeu"""
    return send_file(os.path.join(GAME_DIR, 'index.html'))

@app.route('/<path:filename>')
def serve_game_files(filename):
    """Sert tous les fichiers statiques (js, wasm, data, etc.)"""
    return send_from_directory(GAME_DIR, filename)

@app.after_request
def add_headers(response):
    """Ajoute les headers nécessaires pour WebAssembly et CORS"""
    # Headers pour WebAssembly
    if response.mimetype == 'application/wasm':
        response.headers['Content-Type'] = 'application/wasm'

    # Headers CORS pour le développement local
    response.headers['Access-Control-Allow-Origin'] = '*'
    response.headers['Access-Control-Allow-Methods'] = 'GET, OPTIONS'
    response.headers['Access-Control-Allow-Headers'] = 'Content-Type'

    # Headers de cache pour améliorer les performances
    if any(response.direct_passthrough or response.mimetype == mt for mt in
           ['application/wasm', 'application/octet-stream', 'application/javascript']):
        response.headers['Cache-Control'] = 'public, max-age=3600'

    return response

@app.errorhandler(404)
def not_found(e):
    """Gestion des erreurs 404"""
    return f"Fichier non trouvé. Assurez-vous que le jeu est compilé dans le dossier 'tmx/'", 404

if __name__ == '__main__':
    # Vérifier que le dossier tmx existe
    if not os.path.exists(GAME_DIR):
        print(f"ERREUR: Le dossier '{GAME_DIR}' n'existe pas!")
        print("Veuillez compiler le jeu avec: cd tmx && ./build_web.sh")
        exit(1)

    # Vérifier que les fichiers essentiels existent
    required_files = ['index.html', 'jeu.js', 'jeu.wasm', 'jeu.data']
    missing_files = [f for f in required_files if not os.path.exists(os.path.join(GAME_DIR, f))]

    if missing_files:
        print(f"AVERTISSEMENT: Fichiers manquants: {', '.join(missing_files)}")
        print("Veuillez compiler le jeu avec: cd tmx && ./build_web.sh")

    print("=" * 60)
    print("  Jean-Mi Gounix Simulator - Serveur Web Flask")
    print("=" * 60)
    print(f"  Serveur démarré sur: http://localhost:8000")
    print(f"  Dossier du jeu: {GAME_DIR}")
    print("=" * 60)
    print("  Appuyez sur Ctrl+C pour arrêter le serveur")
    print("=" * 60)

    # Lancer le serveur Flask
    app.run(host='0.0.0.0', port=8000, debug=True)
