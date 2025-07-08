# 01 - Environnement de développement 

**Choix techniques :**

- **Langage principal** : C++
- **Bibliothèque graphique** : OpenGL (avec GLFW, GLAD)
- **Outils de construction** : CMake
- **Éditeur utilisé** : Visual Studio Code
- **Système d’exploitation de développement** : Linux Ubuntu 22.04 / Windows 11

**Objectifs :**

- Avoir un environnement multiplateforme, scriptable et automatisable
- Travailler avec un outil robuste de compilation multi-plateforme (CMake)
- Pouvoir tester localement les performances OpenGL

**Structure du projet :**

- `src/` : code source du moteur (rendu, chargement de mesh, shaders, etc.)
- `include/` : headers publics
- `tests/` : tests unitaires et shaders de test
- `assets/` : modèles et textures pour les démos

**Outils qualité & performance :**

- Valgrind (détection de fuites mémoire)
- RenderDoc (analyse des appels GPU)
- clang-tidy (analyse statique du code)
- GitHub Actions (CI)
