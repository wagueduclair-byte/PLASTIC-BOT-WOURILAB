# Architecture et câblage indicatif
## Chaîne
Entrée unique → zone caméra → classification validée → goulotte orientable → mini-bac organique / recyclable / résiduel.

Le capteur de présence réveille seulement le système.

## Rôles
- ESP32 DevKit : machine à états, capteurs, écran et servo.
- Caméra : acquisition d'image.
- Classificateur : catégorie + confiance; modèle encore à entraîner/intégrer.
- Servo : orientation vers une destination calibrée.

## Broches proposées (à vérifier)
| Fonction | GPIO |
|---|---:|
| Signal servo goulotte | 18 |
| TRIG HC-SR04 | 5 |
| ECHO via adaptation 5 V vers 3,3 V | 19 |
| OLED SDA (facultatif) | 21 |
| OLED SCL (facultatif) | 22 |
| Bouton simulation | 27 |

Alimentation servo séparée, dimensionnée pour le courant de blocage, avec masse commune. Vérifier les broches de démarrage de la carte avant câblage.

## États
IDLE → DETECTED → CLASSIFYING → VALIDATED → POSITIONING → RELEASE → RETURN → IDLE. Résultat invalide ou panne : SAFE_ERROR, sans tri.

## Contrat de réponse (exemple fictif)
```json
{"category":"recyclable","confidence":0.93}
```
Catégories permises : organic, recyclable, residual. Seuil à fixer par essais; l'exemple n'est pas une performance mesurée.
