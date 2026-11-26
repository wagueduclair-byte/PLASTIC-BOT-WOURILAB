# Nomenclature matérielle (BOM)

| Élément | Fonction | État |
|---|---|---|
| ESP32 DevKit V1 | Contrôle principal | Déjà possédé |
| ESP32-CAM AI Thinker | Image de l'objet | À acheter / vérifier |
| HC-SR04 | Distance de présentation | À acheter |
| Capteur IR | Détection complémentaire | À acheter |
| Servo SG90 | Couvercle de maquette | À acheter; couple à vérifier |
| OLED I2C | Affichage | À acheter |
| Buzzer + LEDs | Signalisation | À acheter |
| HX711 + cellule 5 kg | Mesure de poids / remplissage expérimental | Option de mesure |
| Carte LoRa DevKit | Télémétrie | Référence à confirmer |
| Alimentation 5 V régulée | Alimentation des modules | À dimensionner |
| Convertisseur DC-DC | Adaptation d'alimentation | À vérifier |
| Bac, MDF/acrylique, charnières | Structure | À fabriquer / acheter |

## Notes de câblage
- HC-SR04 : TRIG vers une sortie ESP32; ECHO via diviseur de tension vers une entrée ESP32.
- OLED : bus I2C; vérifier l'adresse et les broches de la carte choisie.
- Servo : alimentation 5 V séparée adaptée, masse commune avec ESP32, signal sur GPIO.
- ESP32-CAM : alimentation stable; les pointes de courant peuvent provoquer des redémarrages.
- LoRa : ne pas câbler avant confirmation du modèle et de sa tension logique.
