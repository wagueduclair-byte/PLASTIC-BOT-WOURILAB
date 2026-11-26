# Simulation Wokwi — PLASTIC-BOT WOURILAB

## Ouvrir la simulation
1. Ouvrir le dossier `simulation/wokwi` dans Wokwi (ou importer les fichiers du dossier dans un nouveau projet ESP32).
2. Le diagramme complet est dans `diagram.json`.
3. Le programme de simulation est dans `sketch.ino`.
4. Les bibliothèques requises sont listées dans `libraries.txt`.

## Composants représentés
- ESP32 DevKit C V4
- PIR : présence humaine
- HC-SR04 : objet présenté
- Bouton vert : simulation de la réponse positive de la caméra IA (ce bouton n'existe pas sur la poubelle réelle)
- Servo 1 : couvercle
- Servo 2 : volet de tri
- OLED SSD1306
- HX711 + cellule de charge 5 kg
- LEDs verte/rouge avec résistances de 220 ohms
- Buzzer

## Scénario de démonstration
1. Démarrer la simulation.
2. Cliquer sur le capteur PIR pour simuler une personne devant la poubelle.
3. Modifier la distance du HC-SR04 pour présenter un objet à moins de 18 cm.
4. Pendant les 4 secondes d'analyse, appuyer sur le bouton vert « SIMULER PLASTIQUE » : le système accepte, ouvre le couvercle, actionne le volet, puis affiche le poids.
5. Ne pas appuyer sur le bouton vert : le système refuse et le couvercle reste fermé.
6. Modifier la charge du HX711 depuis les contrôles du composant pour tester la pesée.

## Limite assumée
Wokwi ne simule pas l'inférence d'image de l'ESP32-CAM. Le bouton vert est un substitut de test, explicitement identifié. Sur le prototype réel, la caméra doit renvoyer un JSON de décision avec `accepted` et `confidence`. La vraie caméra, le Wi-Fi et le modèle IA sont intégrés dans `firmware/esp32/esp32_controller.ino` via l'endpoint HTTP configuré.

## Sécurité électrique (montage réel)
- Le HC-SR04 est alimenté en 5 V; son ECHO doit passer par un diviseur résistif avant le GPIO ESP32 (3,3 V max).
- Ne pas alimenter les servos depuis la sortie 3,3 V de l'ESP32. Prévoir une alimentation 5 V dimensionnée pour les deux servos et relier les masses.
- Le câblage de simulation est fonctionnel pour la logique, mais ne remplace pas le schéma d'alimentation du prototype physique.
- Le facteur de calibration HX711 doit être déterminé avec une masse connue.
