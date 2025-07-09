## 06 - Sécurité & Accessibilité

### Sécurité :

Des premières précautions ont été prises pour éviter certains comportements dangereux :
- Les chemins utilisateurs sont validés avant ouverture de fichiers (pas de `../` non vérifiés)
- Seuls les fichiers de modèles au format prévu sont autorisés (`.fbx`, pas de scripts ou DLLs)
- Le projet est compilé avec des options de sécurité de base (`-Wall`, `-Wextra`)

> L’intégration de flags comme `-fstack-protector` ou `-D_FORTIFY_SOURCE` est envisagée pour renforcer la protection mémoire.

### Accessibilité :

Le prototype ne dispose pas encore d’interface graphique, mais l'interaction via **le clavier uniquement** permet une utilisation simple :
- Navigation sans souris
- Interface console lisible et sobre
- Prévue pour s’adapter à un affichage en haut contraste (via terminal)

> Une réflexion future pourra porter sur des aspects plus poussés d’accessibilité (polices lisibles, contraste, support écran lecteur, etc.)
