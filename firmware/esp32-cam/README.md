# ESP32-CAM AI Thinker — serveur de décision

Ce dossier contient le programme de base du module caméra.

## Contrat avec l'ESP32 principal
L'ESP32 principal appelle `GET http://IP_DE_LA_CAMERA/decision`.

Réponse attendue :
```json
{"accepted":true,"confidence":0.93,"reason":"PLASTIC_CONFIRMED"}
```
Le contrôleur principal n'accepte que `accepted=true` ET une confiance d'au moins 0,80. Toute erreur, réponse invalide ou perte Wi-Fi entraîne un refus.

## État actuel — limitation importante
Le fichier `camera_server.ino` initialise la caméra AI Thinker, capture une image et expose l'endpoint HTTP. Le classifieur est volontairement en mode sûr : il renvoie toujours `accepted=false` avec `MODEL_NOT_INSTALLED`. Le modèle IA n'a pas encore été entraîné ni validé. Il serait dangereux de prétendre que la caméra reconnaît déjà les plastiques.

## Mise en service
1. Installer le cœur ESP32 dans Arduino IDE.
2. Sélectionner AI Thinker ESP32-CAM et renseigner le Wi-Fi.
3. Téléverser `camera_server.ino`.
4. Relever l'adresse IP affichée dans le moniteur série.
5. Reporter cette adresse dans `CAMERA_DECISION_URL` du firmware principal.
6. Entraîner et intégrer un classifieur plastique / non-plastique dans `classifyPlastic()`.
7. Tester les faux positifs et faux négatifs avant d'autoriser `accepted=true`.

Ne pas alimenter l'ESP32-CAM depuis la sortie 3,3 V de l'ESP32 principal. Prévoir une alimentation stable adaptée au module.
