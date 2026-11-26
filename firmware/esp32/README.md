# Firmware ESP32 — contrôleur à trois entrées

Le programme principal commande trois servomoteurs indépendants : entrée organique, entrée recyclable et entrée résiduelle.

## Brochage proposé
| Fonction | GPIO |
|---|---:|
| PIR OUT | 27 |
| HC-SR04 TRIG | 25 |
| HC-SR04 ECHO | 26 (diviseur de tension obligatoire) |
| Servo entrée organique | 18 |
| Servo entrée recyclable | 19 |
| Servo entrée résiduelle | 23 |
| HX711 DT | 32 |
| HX711 SCK | 33 |
| OLED SDA / SCL | 21 / 22 |
| LED verte / erreur | 2 / 4 |
| Buzzer | 14 |

Bibliothèques : ESP32Servo, Adafruit GFX, Adafruit SSD1306, HX711 et ArduinoJson 7.

Avant téléversement : renseigner le Wi-Fi et l'adresse IP de l'ESP32-CAM, vérifier les GPIO, régler les angles mécaniques, calibrer la balance et installer un diviseur de tension sur ECHO.

Les trois servos doivent être alimentés par une alimentation externe adaptée au courant de pointe, avec masse commune à l'ESP32. Ne pas utiliser la sortie 3,3 V de la carte.

Le PIR n'ouvre aucune entrée. La caméra doit renvoyer organic, recyclable ou residual avec une confiance minimale de 0,80. Toute panne ou réponse inconnue maintient les trois clapets fermés.

La reconnaissance IA reste à entraîner et à intégrer : le firmware seul ne prouve pas que le modèle est opérationnel. Une seule cellule de charge mesure le poids total de sa plateforme, pas le poids individuel des trois bacs.
