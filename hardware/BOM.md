# Nomenclature matérielle — version trois entrées

| Élément | Qté indicative | Fonction | État |
|---|---:|---|---|
| ESP32 DevKit V1 | 1 | Contrôleur principal | Déjà possédé |
| ESP32-CAM AI Thinker | 1 | Scanner visuel | À acquérir / vérifier |
| HC-SR04 ou capteur de proximité | 1 | Détection de l'objet | À vérifier |
| PIR HC-SR501 | 1 | Détection utilisateur, sans ouverture | Option |
| Servomoteur adapté (MG996R ou équivalent) | 3 | Ouverture indépendante des trois entrées | À dimensionner |
| OLED I2C | 1 | Affichage | À vérifier |
| Buzzer et LEDs | 1 lot | Signalisation | À vérifier |
| Cellule de charge + HX711 | 1 kit | Poids total sur une plateforme | Option |
| Alimentation logique régulée | 1 | ESP32, caméra et capteurs | À dimensionner |
| Alimentation externe servos | 1 | Trois servomoteurs | Obligatoire |
| Fusible, interrupteur et connecteurs | 1 lot | Protection et câblage | À prévoir |
| Châssis et trois bacs | 1 ensemble | Structure | À fabriquer |

Une cellule de charge mesure uniquement le poids de sa plateforme. Pour peser chaque compartiment séparément, prévoir trois cellules et trois modules HX711.

## Règles de câblage
- L'ESP32 utilise une logique 3,3 V ; protéger les GPIO.
- Installer un diviseur de tension sur ECHO du HC-SR04.
- Alimenter les servos par une source externe adaptée au courant de pointe, avec masse commune.
- Ne pas alimenter les servos depuis la broche 3,3 V de l'ESP32.
- Valider le couple des moteurs avec les clapets réels avant achat définitif.
