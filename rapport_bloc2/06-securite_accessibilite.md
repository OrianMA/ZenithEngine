# 06 - Sécurité & Accessibilité

**Mesures de sécurité (OWASP)** :

- Validation stricte des chemins utilisateurs
- Limitation des formats chargés (pas de DLL dynamiques, pas de script)
- Compilation avec les flags `-fstack-protector`, `-D_FORTIFY_SOURCE`

**Accessibilité :**

- Interface compatible clavier uniquement (navigation sans souris)
- Prévue pour adaptation écran contrasté (console, pas de GUI graphique encore)
- Référentiel : OPQUAST – bonnes pratiques fondamentales d’accessibilité