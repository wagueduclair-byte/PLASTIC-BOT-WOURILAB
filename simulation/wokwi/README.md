# Simulation Wokwi — trois entrées

Le circuit simule le contrôleur ESP32, le capteur PIR, le HC-SR04, trois servomoteurs, un OLED, un HX711, les LEDs et le buzzer.

## Boutons de test du classifieur
- GPIO13 : simuler un déchet organique.
- GPIO16 : simuler un déchet recyclable.
- GPIO17 : simuler un déchet résiduel.

Ces boutons remplacent provisoirement la réponse de l'IA. Ils ne sont pas un scanner et ne prouvent pas que la caméra reconnaît les déchets.

## Programme
Le fichier sketch.ino commande uniquement le servomoteur de la catégorie sélectionnée. Le PIR ne commande jamais l'ouverture. Si aucun bouton n'est pressé, aucun clapet ne s'ouvre.

## Limites
La caméra ESP32-CAM et le modèle IA ne sont pas simulés dans ce circuit. Les alimentations réelles des trois servos doivent être séparées et dimensionnées ; le câblage Wokwi n'est pas un schéma d'alimentation à reproduire tel quel sur le prototype.
