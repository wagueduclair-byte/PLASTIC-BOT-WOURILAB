# ESP32-CAM — intégration vision

Cette première version définit le contrat d'intégration. Elle ne contient pas encore de modèle de classification entraîné.

## Contrat HTTP attendu par le contrôleur
L'ESP32 principal appelle `GET /decision`. Le service doit renvoyer :
- `200 application/json` et `{"accepted":true}` si l'objet est reconnu comme plastique autorisé.
- `200 application/json` et `{"accepted":false}` si l'objet est refusé.
- Toute erreur, timeout ou réponse invalide est interprétée comme un refus par le contrôleur.

## Étapes à réaliser
1. Configurer la caméra AI Thinker et vérifier la capture.
2. Créer un jeu d'images étiquetées (plastiques acceptés / autres déchets).
3. Entraîner ou intégrer un modèle léger compatible avec la mémoire disponible, ou déléguer l'inférence à un service local.
4. Mesurer la précision et les faux positifs sur des objets réels, éclairages et angles variés.
5. Ne retourner `accepted:true` qu'au-dessus d'un seuil de confiance validé expérimentalement.

La caméra seule ne reconnaît pas automatiquement le plastique. Ne pas utiliser le prototype pour une collecte réelle avant validation du modèle.
