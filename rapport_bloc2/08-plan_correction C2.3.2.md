# 08 - Plan de correction des bogues

**Méthodologie :**

- Rédaction d’issues GitHub pour chaque anomalie détectée
- Attribution d’un label `bug`
- Priorisation : Critique / Majeur / Mineur
- Chaque bug corrigé est lié à une Pull Request contenant un test associé

**Exemples de bugs traités :**

- Crash au chargement de modèle vide → résolu avec vérification préalable
- Problème de perspective mal calculée → test unitaire pour `Mat4::perspective()`
