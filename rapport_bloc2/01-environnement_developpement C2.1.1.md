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

- Valgrind : non encore utilisé, mais utile pour identifier les fuites mémoire si nécessaire
- RenderDoc outil de profiling graphique repéré pour le debug GPU (à envisager lors de tests visuels poussés)
- clang-tidy : envisagé pour l’analyse statique du code C++ (intégrable dans CMake)
- GitHub Actions : utilisé pour exécuter la CI à chaque commit (compilation + tests unitaires)

Ces outils s’inscrivent dans une démarche qualité, même s’ils ne sont pas tous encore pleinement utilisés dans ce prototype.


