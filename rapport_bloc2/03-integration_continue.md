# 03 - Intégration continue

**Objectif :** tester les modifications en temps réel lors des push ou pull requests

- Git comme système de gestion de sources
- GitHub Actions comme outil CI
- Stratégie : merge via Pull Requests uniquement après validation des tests
- Formatage automatisé via `clang-format`
- Analyse de régression avec test unitaires

**Architecture logicielle :**

- Composants modulaires (rendu, chargement de ressources, interface, etc.)
- Chaque module a ses propres tests