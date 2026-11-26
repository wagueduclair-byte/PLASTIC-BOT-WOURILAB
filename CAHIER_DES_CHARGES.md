# CAHIER DES CHARGES TECHNIQUE ET FONCTIONNEL
## PLASTIC-BOT WOURILAB — Poubelle intelligente à trois entrées

**Version 2.0 — 26 novembre 2026 | Équipe WOURILAB | Douala, Cameroun**

## 1. Résumé du projet
PLASTIC-BOT WOURILAB est une poubelle intelligente équipée de trois entrées indépendantes : déchets organiques, déchets recyclables et déchets résiduels/non recyclables. Avant tout dépôt, l'utilisateur présente l'objet dans une zone de scan. Une caméra capture son image et un système de reconnaissance assisté par intelligence artificielle détermine sa catégorie. Le contrôleur autorise ensuite l'ouverture de la seule entrée correspondante.

En cas d'objet inconnu, d'image illisible, de confiance insuffisante, de panne caméra ou de perte de communication, les trois entrées restent fermées. La détection de présence humaine active uniquement l'interface et ne commande jamais l'ouverture.

## 2. Problématique
Le mélange des déchets organiques, recyclables et résiduels complique leur collecte et leur valorisation. Les poubelles classiques reposent sur le tri manuel et ne vérifient pas le déchet avant le dépôt. Le projet vise à rendre le tri à la source plus simple, contrôlé et accessible.

## 3. Les trois catégories
| Entrée | Catégorie | Exemples indicatifs |
|---|---|---|
| 1 | Organiques / biodégradables | Épluchures, restes alimentaires, feuilles et petits déchets végétaux |
| 2 | Recyclables | Bouteilles et emballages plastiques, papier, carton et canettes métalliques |
| 3 | Résiduels / non recyclables | Déchets souillés ou non valorisables dans les deux autres filières |

Les déchets dangereux, médicaux, électroniques, liquides et coupants sont exclus du périmètre du prototype. La liste exacte des objets acceptés devra être affichée et utilisée pour constituer les données d'entraînement.

## 4. Objectif général et objectifs spécifiques
Concevoir, fabriquer et tester une poubelle à trois entrées qui identifie la catégorie d'un déchet avant d'autoriser son dépôt. Le prototype devra :
- détecter la présence de l'utilisateur et l'objet présenté ;
- capturer l'image du déchet ;
- classer l'objet dans l'une des trois catégories ;
- n'ouvrir que l'entrée correspondant à la catégorie reconnue ;
- garder les trois entrées fermées en cas de doute ou de panne ;
- afficher la décision sur un écran OLED ;
- permettre le vidage et le nettoyage séparés des trois compartiments ;
- mesurer le poids total sur une plateforme instrumentée si la pesée est retenue.

## 5. Cycle de fonctionnement
1. Mise sous tension : initialisation des cartes, capteurs, caméra, écran et servomoteurs ; les trois entrées sont fermées.
2. Détection de présence : le PIR invite l'utilisateur à présenter un objet. Il ne commande aucune ouverture.
3. Présentation : un capteur de proximité détecte l'objet dans la zone de lecture.
4. Scan : la caméra ESP32-CAM capture l'image.
5. Reconnaissance : le modèle IA retourne organic, recyclable, residual ou unknown, avec un score de confiance.
6. Validation : le contrôleur vérifie la catégorie, le score minimal et la validité de la réponse.
7. Ouverture ciblée : seul le servomoteur de l'entrée sélectionnée est actionné.
8. Dépôt : l'objet est déposé dans le compartiment correspondant ; le clapet se referme.
9. Affichage : l'écran indique la catégorie et le résultat.
10. Retour à l'attente : les trois entrées sont en position fermée.

## 6. Architecture fonctionnelle
Utilisateur → PIR (activation interface seulement) → zone de présentation → détection de l'objet → caméra ESP32-CAM → modèle IA → contrôleur ESP32 → commande d'un seul des trois servomoteurs → compartiment correspondant.

Correspondance :
- organic : entrée 1, bac organique ;
- recyclable : entrée 2, bac recyclable ;
- residual : entrée 3, bac résiduel ;
- unknown, erreur, timeout ou confiance faible : aucune ouverture.

## 7. Composants nécessaires
| Élément | Fonction | État |
|---|---|---|
| ESP32 DevKit V1 | Contrôleur principal | Déjà possédé |
| ESP32-CAM AI Thinker | Scanner visuel | À acquérir/vérifier |
| Capteur HC-SR04 ou équivalent | Détection de l'objet présenté | À vérifier |
| PIR HC-SR501 | Détection de présence | Option utile |
| 3 servomoteurs adaptés | Commande indépendante des trois clapets | À acheter/dimensionner |
| OLED I2C | Affichage | À vérifier |
| Buzzer et LEDs | Signalisation | À vérifier |
| Alimentation logique régulée | Cartes et capteurs | À dimensionner |
| Alimentation servos séparée | Trois actionneurs | Obligatoire, courant de pointe à calculer |
| Cellule de charge + HX711 | Poids total sur une plateforme | Option |
| Châssis et trois bacs | Structure de collecte | À fabriquer |
| Câbles, connecteurs, fusible et interrupteur | Montage et protection | À prévoir |

Une cellule de charge mesure uniquement le poids de la plateforme sur laquelle elle est installée. Pour peser chaque compartiment séparément, il faut trois cellules et trois modules HX711.

## 8. Conception mécanique
La structure doit comprendre trois compartiments amovibles, trois entrées physiques indépendantes et trois clapets commandés séparément. La zone de scan doit être distincte des ouvertures : l'objet est d'abord présenté à la caméra, puis l'entrée autorisée s'ouvre.

Prévoir un châssis robuste et lavable, des supports rigides pour les servomoteurs, des butées mécaniques, un boîtier électronique protégé de l'humidité, un accès de vidage pour chaque bac et une zone de scan avec éclairage constant si nécessaire. Les dimensions, matériaux, angles et couples des moteurs doivent être validés sur une maquette avant fabrication définitive.

## 9. Scanner et intelligence artificielle
La caméra capture l'image mais ne reconnaît pas automatiquement les déchets. Un modèle de classification doit être entraîné et intégré. Les classes prévues sont organic, recyclable, residual et unknown.

Le jeu de données doit contenir des images variées : formes, couleurs, tailles, objets propres ou souillés, orientations et conditions d'éclairage. Les images de test doivent être séparées des images d'entraînement. Le seuil initial proposé est 0,80 ; il devra être ajusté après évaluation réelle. Aucun taux de précision ne doit être annoncé avant des essais mesurés.

Tant que le modèle n'est pas entraîné, intégré et testé, la reconnaissance IA reste une fonctionnalité en développement. Des boutons de simulation ne constituent pas une reconnaissance réelle.

## 10. Logiciel et règles de décision
La caméra transmet au contrôleur une catégorie et un score de confiance. Seules les catégories organic, recyclable et residual sont autorisées. Toute valeur inconnue, réponse invalide, délai dépassé, perte Wi-Fi ou confiance inférieure au seuil entraîne le refus.

Le programme doit empêcher deux cycles simultanés, fermer les clapets après chaque dépôt et initialiser les trois servomoteurs en position fermée au démarrage.

## 11. Alimentation et sécurité
- Les GPIO de l'ESP32 fonctionnent en logique 3,3 V ; aucune entrée ne doit recevoir directement 5 V.
- Le signal ECHO du HC-SR04 doit être adapté par diviseur de tension.
- Les servomoteurs ne doivent pas être alimentés par la sortie 3,3 V de l'ESP32.
- Prévoir une alimentation externe dimensionnée pour les courants de pointe des trois servos.
- Relier les masses lorsque les signaux de commande le nécessitent.
- Installer un fusible, un interrupteur général et des protections mécaniques.
- Protéger les cartes de l'humidité et des déchets.
- En cas de panne, les trois entrées restent ou reviennent fermées.

## 12. Simulation et limites
Wokwi peut tester la logique des trois catégories à l'aide de boutons simulant les réponses du classifieur. Cela ne prouve pas que la caméra reconnaît réellement les déchets. La démonstration web est une maquette visuelle et non une vidéo réelle ni une preuve de fonctionnement matériel.

## 13. Plan de tests
| Test | Scénario | Résultat attendu |
|---|---|---|
| T01 | Mise sous tension | Trois entrées fermées |
| T02 | Présence humaine seule | Aucune ouverture |
| T03 | Déchet organique reconnu | Entrée organique seule ouverte |
| T04 | Déchet recyclable reconnu | Entrée recyclable seule ouverte |
| T05 | Déchet résiduel reconnu | Entrée résiduelle seule ouverte |
| T06 | Objet inconnu | Trois entrées fermées |
| T07 | Confiance sous le seuil | Trois entrées fermées |
| T08 | Caméra absente ou timeout | Trois entrées fermées, erreur affichée |
| T09 | Deux présentations rapprochées | Un seul cycle à la fois |
| T10 | Fin du dépôt | Clapet concerné refermé |
| T11 | Redémarrage | Initialisation en position fermée |
| T12 | Pesée | Poids total stable après calibration |

Consigner pour chaque essai la date, le matériel, la version logicielle, les observations et une photo du montage. Mesurer les faux positifs et faux négatifs du modèle.

## 14. Livrables attendus
1. Cahier des charges mis à jour.
2. Plans mécaniques cotés de la structure à trois entrées.
3. Nomenclature illustrée et budget actualisé.
4. Schéma électronique et brochage validés.
5. Firmware ESP32 et ESP32-CAM mis à jour.
6. Jeu de données et modèle IA à trois classes.
7. Simulation de la logique à trois catégories.
8. Rapport de tests avec résultats mesurés.
9. Photos réelles de fabrication et du prototype.
10. Démonstration finale.

## 15. État d'avancement
La carte ESP32 DevKit V1 est déjà possédée. Le module ESP32-CAM, les trois servomoteurs et les matériaux sont à vérifier dans l'inventaire. La structure à trois entrées reste à fabriquer. Le modèle IA doit être entraîné et évalué. Le budget doit être confirmé par devis locaux.

## 16. Conclusion
PLASTIC-BOT WOURILAB évolue vers une poubelle intelligente à trois entrées qui identifie un déchet avant d'autoriser son dépôt. Le projet associe vision artificielle, électronique embarquée et automatisation pour améliorer le tri à la source et encourager une gestion plus responsable des déchets au Cameroun. Le prototype ne sera déclaré fonctionnel qu'après validation réelle de la reconnaissance, de l'ouverture indépendante et des mécanismes de sécurité.
