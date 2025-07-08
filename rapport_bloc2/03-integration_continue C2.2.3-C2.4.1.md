## 03 - Intégration continue

L’intégration continue (CI) permet d’automatiser les tests et vérifications du projet à chaque modification du code, afin de détecter rapidement les erreurs et garantir une base de code stable.

**Objectif :** tester les modifications en temps réel lors des push

- Git est utilisé comme système de gestion de sources
- GitHub Actions est configuré pour exécuter automatiquement les tests à chaque push
- Pour l’instant, les modifications sont directement poussées sur la branche principale sans passer par des Pull Requests (raison : solo dev)
- Le formatage automatique avec `clang-format` n’est pas encore mis en place, le code est formaté manuellement
- Les tests unitaires permettent d’identifier rapidement les régressions
- Une évolution vers une stratégie avec Pull Requests et formatage automatique est envisagée pour améliorer la qualité du code et le processus collaboratif
