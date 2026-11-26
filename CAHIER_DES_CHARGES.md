# Cahier des charges — PLASTIC-BOT WOURILAB

**Version :** 0.1 — 26 novembre 2026  
**Nature :** prototype pédagogique de poubelle intelligente de tri du plastique.

## 1. Contexte et objectif
Le projet vise à construire une poubelle qui limite le dépôt de déchets non plastiques. Elle doit détecter la présentation d'un objet, vérifier sa nature, puis autoriser l'ouverture uniquement si l'objet est reconnu comme un plastique accepté.

## 2. Fonctionnalités attendues
- Détecter une personne ou un objet dans la zone de présentation.
- Capturer une image de l'objet avec l'ESP32-CAM.
- Transmettre une décision de classification au contrôleur ESP32.
- Ouvrir le couvercle avec un servomoteur uniquement après une réponse positive.
- Garder le couvercle fermé si l'objet est refusé, si la caméra ne répond pas ou si la décision est incertaine.
- Donner un retour à l'utilisateur par LEDs, écran OLED et buzzer.
- Prévoir une télémétrie LoRa pour transmettre ultérieurement l'état et le niveau de remplissage.

## 3. Séquence fonctionnelle
**Attente → détection de proximité → capture image → classification → décision**
- Plastique accepté : ouverture temporisée, puis fermeture.
- Objet non accepté : refus, couvercle fermé.
- Erreur / absence de réponse / faible confiance : refus par défaut, couvercle fermé.

La présence humaine ne suffit donc pas à ouvrir la poubelle. La validation de l'objet est obligatoire.

## 4. Architecture matérielle
- ESP32 DevKit V1 : logique principale, capteurs, servo, affichage et communication.
- ESP32-CAM AI Thinker : capture d'image et point d'intégration pour la reconnaissance.
- HC-SR04 et capteur IR : détection de proximité / présence.
- SG90 : actionnement du couvercle de maquette.
- OLED I2C : état du système.
- LEDs et buzzer : acceptation, refus et défaut.
- Carte LoRa DevKit : télémétrie à distance, référence exacte à confirmer.
- Structure mécanique : bac, couvercle, charnières et support du servo.

## 5. Contraintes et sécurité
- Alimentation 5 V régulée, dimensionnée selon les courants de démarrage du servo et de la caméra.
- Masse commune entre modules lorsque les interfaces le nécessitent.
- Ne jamais alimenter un servo puissant depuis la broche 3,3 V de l'ESP32.
- Adapter les niveaux logiques : l'ECHO du HC-SR04 peut être en 5 V et doit être abaissé avant une entrée ESP32.
- Prévoir une protection mécanique contre le pincement et une commande manuelle d'arrêt.
- Le SG90 convient à une maquette légère; choisir un servo plus robuste après mesure de l'effort du couvercle.
- La classification d'image n'est pas garantie par le simple branchement de la caméra : un modèle entraîné et testé est nécessaire.

## 6. Logiciel
- Firmware principal ESP32 en Arduino C++.
- Firmware ESP32-CAM pour capture et service HTTP local.
- Interface de décision provisoire : endpoint HTTP recevant une commande de validation. Le classificateur réel sera intégré après entraînement.
- Firmware LoRa séparé, à adapter au modèle exact de la carte.

## 7. Critères de réception
1. La présence seule ne provoque aucune ouverture.
2. Un objet déclaré accepté provoque l'ouverture après validation.
3. Un objet refusé laisse le couvercle fermé.
4. Une perte de communication ou une réponse invalide laisse le couvercle fermé.
5. Le couvercle revient en position fermée après temporisation.
6. Les indications visuelles et sonores correspondent à l'état.
7. Les essais sont consignés dans le dossier `tests/`.

## 8. Livrables
- Dossier de conception et nomenclature.
- Schéma de câblage à valider sur table.
- Firmware ESP32, ESP32-CAM et squelette LoRa.
- Simulation Wokwi de la partie contrôleur.
- Procédure de tests et relevé des résultats.
- Photographies réelles des composants et du prototype à ajouter dans `images/`.

## 9. Limites connues
La référence exacte du module LoRa, le modèle de reconnaissance plastique, les dimensions définitives du bac et la puissance du servo ne sont pas encore figés. Ces paramètres doivent être vérifiés avant fabrication finale.
