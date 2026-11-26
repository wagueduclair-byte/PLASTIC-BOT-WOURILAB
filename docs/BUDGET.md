# Budget prévisionnel révisé — PLASTIC-BOT WOURILAB

Tous les prix sont en FCFA. Ce sont des estimations de préparation du projet, à confirmer par devis local.

## 1. Électronique et commande

| Composant | Qté | Prix unitaire | Total |
|---|---:|---:|---:|
| ESP32 DevKit V1 (déjà possédé) | 1 | 0 à débourser | 0 |
| ESP32-CAM AI Thinker | 1 | 12 000 | 12 000 |
| HC-SR04 | 1 | 1 000 | 1 000 |
| Capteur IR/PIR | 1 | 1 500 | 1 500 |
| OLED I2C | 1 | 4 000 | 4 000 |
| HX711 | 1 | 1 500 | 1 500 |
| Cellule de charge 5 kg | 1 | 4 500 | 4 500 |
| Buzzer | 1 | 500 | 500 |
| LEDs | 3 | 150 | 450 |
| Lot de résistances | 1 | 500 | 500 |
| MG996R (couvercle) | 1 | 4 000 | 4 000 |
| SG90 (trappe) | 1 | 1 500 | 1 500 |
| **Sous-total** | | | **31 950** |

## 2. Alimentation, câblage et prototypage

| Composant | Qté | Prix unitaire | Total |
|---|---:|---:|---:|
| Alimentation 5 V / 3 A pour électronique | 1 | 5 000 | 5 000 |
| Alimentation séparée adaptée au servo | 1 | 5 000 | 5 000 |
| Convertisseur DC-DC | 1 | 1 500 | 1 500 |
| Breadboard | 1 | 1 000 | 1 000 |
| Fils Dupont | 1 lot | 1 000 | 1 000 |
| Câble USB | 1 | 1 000 | 1 000 |
| Interrupteur | 1 | 500 | 500 |
| Porte-fusible et fusible | 1 | 1 000 | 1 000 |
| Connecteurs | 1 lot | 1 000 | 1 000 |
| **Sous-total** | | | **17 000** |

## 3. Structure mécanique fabriquée par l'équipe

| Matériau / pièce | Qté | Prix unitaire | Total |
|---|---:|---:|---:|
| MDF / contreplaqué | 1 lot | 8 000 | 8 000 |
| Acrylique | 1 lot | 5 000 | 5 000 |
| Filament PLA / pièces imprimées | 1 lot | 6 000 | 6 000 |
| Visserie | 1 lot | 2 000 | 2 000 |
| Charnières et supports | 1 lot | 2 000 | 2 000 |
| Adhésif / colle | 1 lot | 1 500 | 1 500 |
| Deux bacs intérieurs | 2 | 2 000 | 4 000 |
| **Sous-total** | | | **28 500** |

## 4. Outils et consommables d'atelier

| Poste | Montant |
|---|---:|
| Fer à souder | 5 000 |
| Étain et consommables | 1 500 |
| Pistolet à colle et bâtons | 2 500 |
| Outils de coupe/perçage et petites fournitures | 5 000 |
| **Sous-total** | **14 000** |

## 5. Éléments complémentaires

| Poste | Montant |
|---|---:|
| Transport et approvisionnement | 4 000 |
| Petites pièces, protections et finitions | 3 000 |
| Imprévus d'assemblage et essais | 3 000 |
| **Sous-total** | **10 000** |

## 6. Récapitulatif

| Poste | Montant |
|---|---:|
| Électronique et commande | 31 950 |
| Alimentation et câblage | 17 000 |
| Structure mécanique | 28 500 |
| Outils et consommables | 14 000 |
| Compléments | 10 000 |
| **Sous-total** | **101 450** |
| Réserve de 10 % | 10 145 |
| **TOTAL PRÉVISIONNEL** | **111 595 FCFA** |

L'ESP32 DevKit V1 est déjà possédé et n'est pas à racheter. Une provision de 5 000 FCFA pour une alimentation séparée du servo a été ajoutée par sécurité. Le MG996R peut présenter un courant de pointe élevé : il ne doit pas être alimenté par la broche 3,3 V de l'ESP32.

## 7. Prix locaux repérés à titre de comparaison

Le catalogue DBL Électronique à Douala affiche notamment :
- HC-SR04 : 1 000 FCFA.
- SG90 : 1 500 FCFA.
- MG996R/MG995 : 4 000 FCFA.
- Module radio Ra-01 LoRa : 3 500 FCFA.
- Fils jumpers : 1 000 FCFA.

Catalogue : https://dbl-electronique.com/

Ces prix sont des références de catalogue consultées, pas un devis global réservé. Le prix de l'ESP32-CAM, de l'OLED, de la cellule de charge, des matériaux et de l'alimentation supplémentaire doit être confirmé auprès du vendeur avant achat.

## 8. Options non incluses dans ce total

- Deuxième cellule de charge et deuxième HX711 si l'équipe veut mesurer séparément le poids de chaque compartiment.
- Deuxième module LoRa pour une liaison radio point à point.
- Frais éventuels de fabrication spécialisée, découpe laser ou transport supplémentaire.
- Coût d'un entraînement IA externalisé ou d'un service cloud, si l'équipe choisit cette solution.

Ne pas présenter 71 000 FCFA comme le coût complet du prototype : ce montant ne couvre pas la nomenclature révisée ci-dessus.
