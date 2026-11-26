# Cahier des charges — PLASTIC-BOT WOURILAB
**Version 2.0 — novembre 2026 — prototype étudiant en conception**

## 1. Objectif
Concevoir une poubelle intelligente qui pré-trie les déchets au moyen d'une entrée commune, d'une caméra de reconnaissance et d'un mécanisme de répartition interne.

## 2. Architecture physique imposée
- Un seul grand bac extérieur.
- Une seule ouverture de dépôt.
- Une zone où le déchet est présenté à la caméra.
- Une goulotte ou un déflecteur orientable placé après la zone de reconnaissance.
- Trois mini-bacs amovibles à l'intérieur du grand bac.
- Un accès de maintenance pour vider les mini-bacs.

**Aucune ouverture extérieure indépendante par catégorie :** tous les déchets passent par la même entrée et sont ensuite répartis à l'intérieur.

## 3. Catégories proposées
1. Organique (ex. épluchures et restes végétaux).
2. Recyclable (ex. bouteilles PET, canettes, carton propre selon les filières locales).
3. Résiduel (déchets non acceptés dans les deux catégories précédentes).

La liste des matériaux acceptés devra être vérifiée localement. Le système ne doit pas prétendre reconnaître tous les déchets.

## 4. Séquence fonctionnelle
1. L'utilisateur dépose un objet par l'entrée unique.
2. Le capteur de présence réveille le système.
3. La caméra acquiert l'image dans la zone d'inspection.
4. Le classificateur renvoie une catégorie et un score de confiance.
5. Le contrôleur vérifie la catégorie, le seuil de confiance et l'état du mécanisme.
6. La goulotte se place vers le mini-bac correspondant.
7. Le déchet est libéré, puis la goulotte revient au neutre.

Le capteur de présence ne commande jamais directement le tri.

## 5. Exigences fonctionnelles
- EF01 : une seule ouverture de dépôt.
- EF02 : détection de présence sans décision de catégorie.
- EF03 : acquisition d'image exploitable.
- EF04 : classification parmi les catégories effectivement entraînées.
- EF05 : orientation vers trois destinations internes distinctes.
- EF06 : affichage des états : attente, analyse, tri, erreur.
- EF07 : retrait séparé des mini-bacs.
- EF08 : refus de tri si catégorie inconnue ou confiance insuffisante.
- EF09 : retour à un état sûr après panne ou coupure.

## 6. Sécurité
- Classification absente, ambiguë ou trop peu fiable : aucun mouvement de tri.
- Prévoir une position neutre ou un compartiment de rejet à définir.
- Protéger les doigts des pièces mobiles.
- Immobiliser le mécanisme avant intervention manuelle.
- Alimenter les servos séparément, avec masse commune à l'ESP32.
- Protéger l'électronique des liquides et déchets.
- Prévoir un arrêt manuel accessible.

## 7. Électronique indicative
- ESP32 DevKit V1 : commande centrale.
- ESP32-CAM ou caméra compatible : acquisition d'image (à acquérir/confirmer).
- Capteur de proximité : détection d'approche.
- Un ou plusieurs servomoteurs : goulotte et éventuellement retenue du déchet.
- OLED facultatif : messages d'état.
- Alimentation régulée dimensionnée pour les moteurs.

Le nombre de servos et leur couple dépendent de la masse, du bras de levier et de la géométrie de la goulotte. À déterminer sur maquette.

## 8. Limites et hypothèses
Les dimensions, matériaux, angles, modèle de caméra et couple moteur ne sont pas encore figés. Une ESP32-CAM seule n'est pas un classificateur IA : un modèle doit être entraîné, embarqué ou exécuté sur un ordinateur/serveur local. Wokwi valide la logique de commande, pas la vision ni la mécanique.

## 9. Critères de validation
- Tous les déchets entrent par l'ouverture unique.
- Les trois mini-bacs sont atteints sur la maquette.
- Un objet inconnu provoque un état sûr.
- Aucun mouvement avant validation de la classification.
- L'alimentation moteur ne provoque pas de redémarrage.
- Les essais sont consignés avec photos et résultats.

## 10. Livrables
Maquette, schéma de câblage, firmware, module de classification évalué, démonstration, plan de tests et budget appuyé par des devis réels.
