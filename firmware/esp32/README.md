# Firmware ESP32 DevKit V1

## Programme
- `esp32_controller.ino` : programme principal du prototype réel.
- Il gère le PIR, le HC-SR04, les deux servos, l'OLED, le HX711, les LEDs et le buzzer.
- Il interroge l'ESP32-CAM par HTTP pour la classification.

## Brochage proposé
| Fonction | GPIO |
|---|---:|
| PIR OUT | 27 |
| HC-SR04 TRIG | 25 |
| HC-SR04 ECHO | 26 (diviseur 5V -> 3,3V obligatoire) |
| Servo couvercle | 18 |
| Servo volet de tri | 19 |
| HX711 DT | 32 |
| HX711 SCK | 33 |
| OLED SDA | 21 |
| OLED SCL | 22 |
| LED verte | 2 |
| LED rouge | 4 |
| Buzzer | 14 |

## Bibliothèques
- ESP32Servo
- Adafruit GFX Library
- Adafruit SSD1306
- HX711 (Bogdan Necula)
- ArduinoJson 7

## Avant le téléversement
1. Remplacer le SSID et le mot de passe Wi-Fi.
2. Remplacer l'IP dans `CAMERA_DECISION_URL` par celle de l'ESP32-CAM.
3. Calibrer la cellule avec une masse connue et corriger `FACTEUR_CALIBRATION`.
4. Vérifier les angles mécaniques des deux servos.
5. Alimenter les servos par une alimentation 5 V séparée dimensionnée pour leurs courants de pointe, avec masse commune à l'ESP32.
6. Installer un diviseur résistif sur ECHO du HC-SR04 (la broche ESP32 n'est pas tolérante au 5 V).

## Logique de sécurité
Le PIR ne commande jamais les servos. Il active seulement la zone. Il faut ensuite un objet détecté par ultrasons, puis une réponse caméra valide avec `accepted=true` et `confidence >= 0.80`. Toute panne conduit au refus et au couvercle fermé.

**Limites non résolues :** modèle IA à entraîner/valider, calibration réelle du poids, validation des angles et essais physiques. Une seule cellule de charge mesure le poids total sur sa plateforme, pas le poids séparé des deux compartiments.
