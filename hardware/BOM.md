# Nomenclature prévisionnelle
Les quantités sont indicatives. Vérifier les références, dimensions et prix avant commande.

| Élément | Qté indicative | Fonction | État |
|---|---:|---|---|
| ESP32 DevKit V1 | 1 | Contrôle principal | Déclaré disponible |
| ESP32-CAM + adaptateur USB-série | 1 | Capture d'image | À acquérir/confirmer |
| Capteur de proximité HC-SR04 ou ToF | 1 | Détection | À confirmer |
| Servomoteur à couple adapté | 1 à 2 | Orientation/retention | À dimensionner |
| Alimentation régulée moteurs | 1 | Puissance servos | À dimensionner |
| Régulateur logique | 1 | Alimentation stable | À confirmer |
| OLED I2C | 1 facultatif | Affichage | À confirmer |
| Bouton d'arrêt/validation | 1 à 2 | Sécurité et essais | À prévoir |
| Grand bac extérieur | 1 | Châssis | À fabriquer/acheter |
| Mini-bacs intérieurs | 3 | Collecte séparée | À fabriquer/acheter |
| Goulotte, axe et supports | 1 ensemble | Répartition | À fabriquer |
| Câbles, borniers, visserie, protections | 1 lot | Assemblage | À prévoir |

## Dimensionnement
- Le couple servo dépend du poids et du bras de levier de la goulotte.
- Ne pas alimenter les servos depuis le 3,3 V de l'ESP32.
- HC-SR04 ECHO est généralement en 5 V : adapter le niveau avant GPIO ESP32.
- Pour peser les trois bacs séparément : trois cellules de charge et trois HX711. Une seule cellule mesure uniquement le poids à son emplacement.
- Les capteurs de remplissage sont une option ultérieure.
