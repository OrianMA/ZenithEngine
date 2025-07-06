# ZenithEngine

## 1. Introduction
ZenithEngine est un moteur de rendu 3D minimaliste écrit en C++ utilisant OpenGL 3.3. Il permet de charger et d'afficher des modèles 3D au format glTF, avec gestion des shaders, textures, caméras et éclairage de base. Le projet est conçu pour être pédagogique, modulaire et facilement extensible, idéal pour l'apprentissage ou comme base pour des projets graphiques plus avancés.

## 2. Installation, Compilation et Test

### Prérequis
- CMake >= 3.20
- Un compilateur C++ compatible C++20
- [vcpkg](https://github.com/microsoft/vcpkg) pour la gestion des dépendances
- OpenGL 3.3+

### Dépendances principales (gérées par vcpkg)
- glfw3
- glad
- nlohmann_json

### Installation des dépendances
Dans le dossier racine du projet :
```sh
vcpkg install glfw3 glad nlohmann-json
```

### Compilation
1. Cloner le dépôt et ouvrir un terminal dans le dossier `ZenithEngine`.
2. Générer les fichiers de build avec CMake :
   ```sh
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
   ```
3. Compiler :
   ```sh
   cmake --build build
   ```
4. L'exécutable sera généré dans `build/` ou selon la configuration de votre IDE.

### Lancement
Placez les modèles glTF (ex : `scene.gltf` et ses ressources) dans `ZenithEngine/ZenithEngine/Models/` puis lancez l'exécutable :
```sh
./build/ZenithEngine
```

### Test
Aucun test automatisé n'est fourni pour l'instant. Le test principal consiste à lancer l'application et vérifier l'affichage du modèle 3D.

## 3. Vue d'ensemble de l'architecture du code

Le cœur du moteur est organisé autour de plusieurs classes principales :

- **Model** : Gère le chargement, le parsing (via nlohmann_json) et le rendu des modèles glTF. Il traverse la hiérarchie de nœuds et instancie les Mesh associés.
- **Mesh** : Représente une géométrie 3D, encapsule les buffers OpenGL (VAO, VBO, EBO) et les textures.
- **Shader** : Gère la compilation, l'activation et la suppression des programmes de shaders OpenGL (vertex/fragment).
- **Camera** : Gère la vue, la projection et les contrôles utilisateur (déplacement, orientation).
- **Texture** : Charge et gère les textures associées aux modèles.

L'entrée principale du programme (`ZenithEngine.cpp`) :
- Initialise GLFW/GLAD et la fenêtre OpenGL
- Charge un modèle glTF
- Configure la caméra et l'éclairage
- Boucle de rendu : gestion des entrées, mise à jour de la caméra, dessin du modèle

Arborescence simplifiée :
```
ZenithEngine/
  ├── ZenithEngine.cpp         # Point d'entrée principal
  ├── Model.h/.cpp            # Gestion des modèles glTF
  ├── Mesh.h/.cpp             # Gestion des maillages
  ├── shaderClass.h/.cpp      # Gestion des shaders
  ├── Camera.h/.cpp           # Caméra et contrôles
  ├── Texture.h/.cpp          # Textures
  ├── Models/                 # Modèles 3D glTF
  └── ...
```

## 4. Mini-guide utilisateur & développeur

### Utilisateur
- Placez vos modèles glTF dans `ZenithEngine/ZenithEngine/Models/`.
- Par défaut, le fichier chargé est `Models/statue/scene.gltf` (modifiable dans `ZenithEngine.cpp`).
- Contrôles caméra : utilisez la souris et le clavier (voir implémentation dans `Camera.cpp`).
- L'application affiche le FPS dans le titre de la fenêtre.

### Développeur
- Pour ajouter un nouveau modèle, modifiez la ligne :
  ```cpp
  Model model("Models/statue/scene.gltf");
  ```
- Pour ajouter des fonctionnalités (nouveaux shaders, effets, contrôles), étendez les classes existantes ou ajoutez-en de nouvelles.
- Les shaders sont dans des fichiers `.vert` et `.frag` (ex : `default.vert`, `default.frag`).
- Les dépendances sont gérées via vcpkg et CMake.

### Limitations connues
- Seul le format glTF est supporté pour les modèles.
- Pas de gestion avancée des animations ou des matériaux complexes.
- Pas de tests unitaires intégrés.

---

Pour toute contribution ou question, ouvrez une issue ou une pull request sur le dépôt GitHub. 