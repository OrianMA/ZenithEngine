# 02 - Déploiement continu

**Objectif :** permettre une mise à disposition continue du moteur via GitHub

- Utilisation de **GitHub Actions** pour :
  - Compiler automatiquement sur Linux & Windows
  - Générer des artefacts (archives compressées)
  - Exécuter les tests

**Étapes du pipeline :**

1. Checkout du dépôt
2. Configuration via CMake
3. Compilation Debug + Release
4. Exécution des tests unitaires
5. Upload des binaires en artifacts

**Critères de qualité :**

- Le pipeline échoue si les tests ne passent pas
- Les artefacts doivent être exploitables manuellement
