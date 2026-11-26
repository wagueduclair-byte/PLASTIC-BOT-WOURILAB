# PLASTIC-BOT WOURILAB

Prototype de poubelle intelligente qui contrôle l'ouverture après reconnaissance d'un déchet plastique, oriente le déchet vers un compartiment et mesure sa masse.

## Documents principaux

- **[CAHIER DES CHARGES COMPLET](CAHIER_DES_CHARGES.md)** — fonctionnement détaillé, architecture, composants illustrés, fabrication, sécurité, tests et budget.
- [Galerie photos des composants](PHOTOS_COMPOSANTS.md) — photographies de référence des modules et pièces.
- [Budget détaillé](docs/BUDGET.md) — nomenclature et estimation prévisionnelle en FCFA.
- [Cahier des charges dans docs](docs/CAHIER_DES_CHARGES.md).

## Fonctionnement essentiel

1. Détection de l'utilisateur : le système passe en mode présentation, sans ouvrir.
2. Détection de l'objet présenté.
3. Capture par ESP32-CAM et classification par un modèle IA entraîné.
4. Objet non reconnu ou non autorisé : refus, couvercle fermé.
5. Plastique accepté : validation par ESP32, ouverture motorisée, orientation vers le bac.
6. Mesure du poids et affichage.

**La présence humaine seule ne déclenche jamais l'ouverture.** En cas de doute, de panne caméra ou de perte de communication, le système reste fermé.

## État du projet

Le dépôt contient la documentation de conception et les bases logicielles. Le modèle IA doit être entraîné et évalué ; les plans mécaniques cotés, le brochage final et les photographies du prototype réellement fabriqué seront ajoutés après validation et assemblage. Les photos dans la galerie sont des références de composants, pas des preuves d'achat.

## Budget

Le budget prévisionnel révisé est de **111 595 FCFA**, réserve de 10 % incluse, avec ESP32 DevKit V1 déjà possédé et non à racheter. Les montants doivent être confirmés par devis avant les achats.
