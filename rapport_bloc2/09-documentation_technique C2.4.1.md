# 09 - Documentation technique

**Manuel de déploiement :**

- Prérequis : CMake, compilateur C++, OpenGL >= 3.3
- Cloner le repo → `mkdir build && cd build && cmake .. && make`

**Manuel d’utilisation :**

#### Compilation via Visual Studio (ou VS Code) :
- Ouvrir le dossier du projet dans Visual Studio
- Laisser CMake configurer automatiquement
- Sélectionner la cible `Debug` ou `Release`
- Lancer la compilation (F7)
- Lancer l’exécutable depuis l’IDE (Ctrl + F5)

#### Compilation en ligne de commande (alternative) :
```bash
git clone https://github.com/OrianMA/ZenithEngine.git
cd ZenithEngine\ZenithEngine
mkdir build && cd build
cmake ..
make
```
- ZQSD pour se déplacer
- ESC pour quitter

**Manuel de mise à jour :**

- Pull dernière version GitHub
- Supprimer et régénérer le dossier `build`

**Choix techniques :**

- Langage : C++
- API graphique : OpenGL 3.3 Core
- Outils : CMake, Git, GoogleTest, GitHub Actions
- Dépendances : GLFW, GLAD, stb_image
