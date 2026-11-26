# Simulation Wokwi

La première simulation doit couvrir le contrôleur ESP32, le HC-SR04, un servo, les LEDs et le buzzer.

## Montage à reproduire
- ESP32 DevKit V1
- HC-SR04 : TRIG GPIO5, ECHO GPIO18 (simulation; sur matériel réel, diviseur de tension obligatoire)
- Servo : signal GPIO19, alimentation adaptée
- LED verte GPIO2, LED rouge GPIO4
- Buzzer GPIO23

## Scénarios
1. Distance supérieure à 18 cm : aucune action.
2. Objet détecté mais validation caméra absente : le couvercle reste fermé.
3. Réponse caméra positive : ouverture pendant 2,5 s puis fermeture.
4. Réponse négative, timeout ou Wi-Fi indisponible : couvercle fermé.

Wokwi ne reproduit pas la reconnaissance d'image de l'ESP32-CAM. Simuler cette réponse avec un faux serveur ou une fonction de test, puis valider la caméra sur matériel réel.
