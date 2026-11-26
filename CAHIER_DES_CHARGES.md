# CAHIER DES CHARGES TECHNIQUE ET FONCTIONNEL
## PLASTIC-BOT WOURILAB
### Poubelle intelligente de reconnaissance et de tri des déchets plastiques

**Version :** 1.0 — 26 novembre 2026  
**Porteur du projet :** équipe WOURILAB  
**Domaine :** génie électrique, électronique embarquée, IoT, vision artificielle et écologie  
**Lieu de réalisation :** Douala, Cameroun

---

# 1. RÉSUMÉ EXÉCUTIF

PLASTIC-BOT WOURILAB est une poubelle intelligente conçue et fabriquée par l'équipe. Elle ne s'ouvre pas simplement lorsqu'une personne s'approche : elle attend qu'un utilisateur présente un déchet, vérifie sa nature à l'aide d'une caméra et d'un système de reconnaissance, puis autorise l'ouverture uniquement si l'objet est identifié comme un plastique accepté.

Après validation, un premier servomoteur commande le couvercle. Un mécanisme interne orientable, commandé par un second servomoteur, dirige ensuite le déchet vers le compartiment prévu. Un système de pesée basé sur une cellule de charge et le convertisseur HX711 mesure la masse déposée. Un écran OLED affiche les informations utiles.

**Règle fondamentale :** présence humaine ≠ ouverture. L'ouverture dépend obligatoirement de la validation du déchet. En cas de refus, d'incertitude ou de panne de communication, le couvercle reste fermé.

# 2. CONTEXTE ET PROBLÉMATIQUE

Les déchets plastiques abandonnés dans les rues, les caniveaux et les espaces publics contribuent à la pollution et à l'obstruction des systèmes d'évacuation des eaux. Les poubelles classiques ne vérifient pas la nature des déchets qui y sont déposés et ne facilitent pas toujours le tri.

Le projet répond à la question suivante :

**Comment concevoir une poubelle autonome capable de reconnaître un déchet plastique avant d'autoriser son dépôt, de l'orienter vers un compartiment et d'enregistrer sa masse ?**

# 3. OBJECTIFS

## 3.1 Objectif général
Concevoir, fabriquer et tester un prototype de poubelle intelligente qui contrôle l'accès au bac en fonction de la reconnaissance du déchet et automatise le tri et le suivi du poids.

## 3.2 Objectifs spécifiques
- Détecter l'arrivée d'un utilisateur dans la zone de dépôt.
- Détecter la présentation d'un objet dans la zone d'analyse.
- Capturer une image avec l'ESP32-CAM.
- Classifier l'objet à l'aide d'un modèle de vision entraîné.
- Refuser les objets non plastiques ou non reconnus.
- Ouvrir automatiquement le couvercle uniquement après validation.
- Orienter le déchet accepté vers le compartiment correspondant.
- Mesurer le poids avec une cellule de charge et un module HX711.
- Afficher l'état, le résultat de reconnaissance et le poids sur OLED.
- Prévoir une transmission distante des données par LoRa, après choix et intégration du module.

# 4. DESCRIPTION GÉNÉRALE DU SYSTÈME

Le prototype comprend :
1. Une structure mécanique fabriquée par l'équipe.
2. Une zone de présentation et de prise d'image.
3. Un bloc électronique de commande autour de l'ESP32 DevKit V1.
4. Un module caméra ESP32-CAM.
5. Un mécanisme de couvercle motorisé.
6. Une trappe ou goulotte d'orientation vers deux compartiments.
7. Un dispositif de pesée.
8. Un écran d'information, des voyants et un avertisseur sonore.

## 4.1 Schéma fonctionnel simplifié

```text
UTILISATEUR
    |
    v
Détection de présence (PIR / IR)
    |
    v
Invitation à présenter un déchet
    |
    v
Détection de l'objet (proximité)
    |
    v
Caméra ESP32-CAM : capture de l'image
    |
    v
Modèle de reconnaissance du déchet
    |
    +---- Objet refusé / incertain / erreur
    |                  |
    |                  v
    |          Couvercle RESTE FERMÉ
    |          LED rouge + buzzer + message OLED
    |
    +---- Plastique accepté
                       |
                       v
              Validation par ESP32
                       |
                       v
              Servo 1 : ouverture du couvercle
                       |
                       v
              Servo 2 : orientation de la trappe
                       |
                       v
              Dépôt dans le compartiment ciblé
                       |
                       v
              Cellule de charge + HX711
                       |
                       v
              Poids / état affichés sur OLED
                       |
                       v
              Fermeture et retour en attente
```

# 5. FONCTIONNEMENT DÉTAILLÉ — CYCLE UTILISATEUR

**Étape 1 — Mise sous tension :** l'ESP32 initialise les capteurs, les servomoteurs, l'écran et la liaison avec la caméra. Le système vérifie que les éléments essentiels répondent. L'écran affiche « PLASTIC-BOT prêt ».

**Étape 2 — Détection de l'utilisateur :** le capteur PIR ou IR détecte une présence dans la zone d'approche. Cette détection ne commande pas l'ouverture. Elle active seulement le mode attente de présentation et peut afficher « Présentez un déchet ».

**Étape 3 — Présentation du déchet :** le capteur de proximité HC-SR04 détecte qu'un objet est placé devant la zone de capture. La caméra est alors sollicitée.

**Étape 4 — Capture et reconnaissance :** l'ESP32-CAM capture une image correctement éclairée et cadrée. Le logiciel de vision compare l'image aux classes apprises par le modèle. Le modèle doit être entraîné sur des images représentatives des déchets rencontrés. La caméra seule ne reconnaît pas automatiquement le plastique sans modèle logiciel.

**Étape 5 — Décision :**
- Si la confiance de classification dépasse le seuil défini et que la classe est autorisée, l'ESP32 reçoit une validation.
- Si l'objet est non plastique, hors catégories, mal cadré ou si la confiance est insuffisante, le dépôt est refusé.
- Si la caméra ou la communication tombe en panne, le système adopte un comportement sûr : couvercle fermé.

**Étape 6 — Ouverture :** après validation seulement, le servomoteur MG996R commande l'ouverture du couvercle. La durée d'ouverture est limitée par le programme.

**Étape 7 — Orientation et tri :** le second servomoteur SG90 actionne une petite trappe orientable. La position dépend de la classe reconnue. Le prototype prévoit deux compartiments internes. La table de correspondance entre classes et compartiments sera fixée après choix des catégories de plastiques retenues et essais mécaniques.

**Étape 8 — Pesée :** la cellule de charge placée sous le support du bac transmet un signal au HX711. L'ESP32 calcule la masse après stabilisation de la mesure. Le poids affiché correspond au poids mesuré dans le compartiment instrumenté ; pour mesurer séparément les deux bacs, chaque compartiment devra disposer de sa propre cellule de charge et de son propre canal de mesure.

**Étape 9 — Information et fermeture :** l'écran indique le résultat et le poids. Le mécanisme revient à sa position initiale et le couvercle se referme. Le système retourne en attente.

# 6. ARCHITECTURE ÉLECTRONIQUE ET RÔLE DES COMPOSANTS

| Élément | Rôle dans le système |
|---|---|
| ESP32 DevKit V1 | Contrôleur principal : capteurs, servos, OLED, pesée et logique de sécurité |
| ESP32-CAM AI Thinker | Capture des images et interface avec le système de reconnaissance |
| PIR / capteur IR | Détection de présence humaine dans la zone d'approche |
| HC-SR04 | Détection de proximité de l'objet présenté |
| MG996R | Motorisation du couvercle principal |
| SG90 | Orientation de la trappe de tri sur maquette légère |
| OLED I2C | Affichage des messages, états et poids |
| Cellule de charge 5 kg | Mesure de la masse |
| HX711 | Amplification et conversion du signal de la cellule de charge |
| Buzzer | Signal sonore d'acceptation, de refus ou d'erreur |
| LEDs rouge/verte | Signalisation visuelle |
| LoRa (option télémétrie) | Transmission à distance après validation de la carte exacte |

# 7. PHOTOGRAPHIES DES COMPOSANTS

Les photographies ci-dessous sont des images de référence de composants physiques provenant de catalogues techniques ou de vendeurs. Elles servent à identifier les pièces à acheter ; elles ne prétendent pas être les photographies des pièces effectivement reçues par l'équipe.

## 7.1 Cartes de commande et de vision

### ESP32 DevKit V1 — déjà possédé
![Photographie ESP32 DevKit V1](https://probots.co.in/pub/media/catalog/product/cache/d8ddd0f9b0cd008b57085cd218b48832/e/s/esp32_30_pin_development_board_5_1.jpg)
- Quantité : 1
- Fonction : commande centrale.
- Achat à prévoir : non, carte déjà possédée.
- Valeur indicative de remplacement : 7 000 FCFA (référence de catalogue local à vérifier).

### ESP32-CAM AI Thinker
![Photographie ESP32-CAM](https://cdn.webshopapp.com/shops/346407/files/420566009/700x700x2/otronic-esp32-cam-development-board-with-camera-wi.jpg)
- Quantité : 1
- Fonction : capture d'image du déchet.
- Prix budgété : 12 000 FCFA.
- Achat à prévoir : oui.

## 7.2 Capteurs

### Capteur ultrason HC-SR04
![Photographie HC-SR04](https://rapidroboticsaustralia.com/cdn/shop/files/HC-SR04Module_3083482b-663a-4f32-b909-4c2e60898439.jpg?v=1726877831&width=2000)
- Quantité : 1
- Fonction : détecter l'objet dans la zone de présentation.
- Prix de référence local : 1 000 FCFA.

### Capteur de présence PIR HC-SR501
![Photographie capteur PIR](https://cricklewoodelectronics.com/images/D/HCSR501-01.jpg)
- Quantité : 1
- Fonction : détecter l'approche de l'utilisateur, sans déclencher l'ouverture.
- Prix budgété pour capteur IR : 1 500 FCFA (à confirmer selon le modèle retenu).

## 7.3 Actionneurs

### Servomoteur MG996R — couvercle
![Photographie servomoteur MG996R](https://zaitronics.com.au/cdn/shop/files/Sfd6ad1b9c62d466992f59318899086920.webp?crop=center&height=1200&v=1712137837&width=1200)
- Quantité : 1
- Fonction : ouvrir et fermer le couvercle.
- Prix de référence local : 4 000 FCFA.
- Alimentation : prévoir une alimentation dédiée adaptée au courant de pointe du servo.

### Servomoteur SG90 — trappe de tri
![Photographie servomoteur SG90](https://unitronic-online.com/web/image/product.product/11151/image_1024/%5BSG90%5D%20Servomotor%20SG90%20180%20Grados%201.3kg?unique=2350359)
- Quantité : 1
- Fonction : orienter la trappe de tri d'une maquette légère.
- Prix de référence local : 1 500 FCFA.
- Limite : ne pas lui confier un mécanisme lourd.

## 7.4 Affichage et pesée

### Écran OLED I2C 0,96 pouce
![Photographie écran OLED](https://cdn.shopify.com/s/files/1/0069/6513/3376/products/AA139_550x.jpg?v=1557283862)
- Quantité : 1
- Fonction : afficher les consignes, décisions et mesures.
- Prix budgété : 4 000 FCFA.

### Cellule de charge 5 kg et module HX711
![Photographie cellule de charge et HX711](https://scccltd.com/cdn/shop/files/hx711-5kg-load-cell-module-kit-front.jpg?v=1762497204&width=1920)
- Quantité : 1 kit
- Fonction : mesurer la masse des déchets déposés.
- Cellule : 5 kg ; module HX711 : conversion du signal.
- Prix budgété séparément : cellule 4 500 FCFA + HX711 1 500 FCFA.
- Pour mesurer deux compartiments séparément : prévoir deux dispositifs de pesée.

## 7.5 Alimentation et prototypage

### Adaptateur 5 V / 3 A
![Photographie alimentation 5V 3A](https://mikrobot.pl/userdata/public/gfx/5707/Zasilacz-impulsowy-wtyczkowy-5V-3A-15W-wtyk-DC-2.1-1.jpg)
- Quantité : 1 pour l'électronique, à compléter par une alimentation adaptée au servo principal.
- Prix budgété : 5 000 FCFA.

### Convertisseur DC-DC abaisseur
![Photographie convertisseur DC DC](https://down-br.img.susercontent.com/file/br-11134207-7qukw-lhtkfx4eruk77a)
- Quantité : 1
- Fonction : adapter une tension d'alimentation.
- Prix budgété : 1 500 FCFA.

### Breadboard, fils Dupont, LEDs, résistances et petits composants
![Photographie kit de prototypage électronique](https://down-tw.img.susercontent.com/file/tw-11134201-81ztl-mf8beyx1rjewf1)
- Fonction : essais, interconnexions, signalisation et montage de la maquette.
- Prix budgétés dans la nomenclature détaillée ci-dessous.

### Module LoRa Ra-01 (télémétrie prévue)
- Quantité : 1 paire de modules pour une liaison point à point, si la transmission distante est retenue.
- Prix de référence local : 3 500 FCFA l'unité.
- À ne pas acheter avant validation de la fréquence, de la carte hôte et de l'architecture radio.

# 8. STRUCTURE MÉCANIQUE ET FABRICATION

La structure complète est fabriquée par l'équipe WOURILAB. Elle n'est pas achetée comme une poubelle commerciale déjà assemblée.

## 8.1 Éléments mécaniques
- Châssis principal en MDF/contreplaqué ou matériau équivalent.
- Habillage et fenêtres de protection en acrylique.
- Couvercle articulé sur charnières.
- Support réglable du servomoteur MG996R.
- Zone de présentation du déchet devant la caméra.
- Goulotte de transfert et trappe orientable.
- Deux compartiments amovibles.
- Support de pesée sous le bac instrumenté.
- Trappes d'accès pour maintenance et vidage.
- Fixations, vis, écrous, entretoises et protections de câbles.

## 8.2 Principe de fabrication
1. Dessiner les dimensions et réaliser le plan coté.
2. Découper les panneaux du châssis.
3. Assembler le corps et installer les deux compartiments.
4. Fixer les charnières et le couvercle.
5. Fabriquer la goulotte et la trappe orientable.
6. Installer les supports des deux servomoteurs.
7. Installer la cellule de charge sous le bac prévu.
8. Installer les capteurs et la caméra dans des supports rigides.
9. Fixer le boîtier électronique à l'écart des déchets et de l'humidité.
10. Effectuer les essais mécaniques à vide puis en charge.

**Important :** les dimensions définitives, le couple du servo et l'épaisseur des panneaux doivent être validés sur le prototype avant découpe finale.

# 9. NOMENCLATURE ET BUDGET PRÉVISIONNEL

Montants indicatifs en FCFA. Les prix du tableau sont les estimations de travail retenues pour le projet, pas des devis fermes. Les prix réellement observés chez un fournisseur local sont signalés dans la colonne source.

## 9.1 Électronique, commande et capteurs

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
| Servomoteur MG996R (couvercle) | 1 | 4 000 | 4 000 |
| Servomoteur SG90 (trappe) | 1 | 1 500 | 1 500 |
| **Sous-total électronique et commande** | | | **31 950** |

## 9.2 Alimentation, câblage et prototypage

| Composant | Qté | Prix unitaire | Total |
|---|---:|---:|---:|
| Alimentation régulée 5 V / 3 A (électronique) | 1 | 5 000 | 5 000 |
| Alimentation séparée adaptée au servo (provision) | 1 | 5 000 | 5 000 |
| Convertisseur DC-DC | 1 | 1 500 | 1 500 |
| Breadboard | 1 | 1 000 | 1 000 |
| Fils Dupont | 1 lot | 1 000 | 1 000 |
| Câble USB | 1 | 1 000 | 1 000 |
| Interrupteur | 1 | 500 | 500 |
| Porte-fusible et fusible | 1 | 1 000 | 1 000 |
| Connecteurs | 1 lot | 1 000 | 1 000 |
| **Sous-total alimentation et câblage** | | | **17 000** |

## 9.3 Structure mécanique

| Matériau / pièce | Qté | Prix unitaire | Total |
|---|---:|---:|---:|
| MDF / contreplaqué | 1 lot | 8 000 | 8 000 |
| Acrylique | 1 lot | 5 000 | 5 000 |
| Filament PLA / pièces imprimées | 1 lot | 6 000 | 6 000 |
| Visserie | 1 lot | 2 000 | 2 000 |
| Charnières et supports | 1 lot | 2 000 | 2 000 |
| Adhésif / colle | 1 lot | 1 500 | 1 500 |
| Deux bacs intérieurs | 2 | 2 000 | 4 000 |
| **Sous-total mécanique** | | | **28 500** |

## 9.4 Outils et consommables d'atelier

| Matériel | Provision |
|---|---:|
| Fer à souder | 5 000 |
| Étain et consommables de soudure | 1 500 |
| Pistolet à colle et bâtons | 2 500 |
| Outils de coupe/perçage et petites fournitures | 5 000 |
| **Sous-total outils** | **14 000** |

## 9.5 Éléments complémentaires

| Poste | Provision |
|---|---:|
| Transport et approvisionnement | 4 000 |
| Petites pièces, protections et finitions | 3 000 |
| Imprévus d'assemblage et essais | 3 000 |
| **Sous-total complémentaire** | **10 000** |

## 9.6 Récapitulatif révisé

| Poste | Montant |
|---|---:|
| Électronique et commande | 31 950 FCFA |
| Alimentation et câblage | 17 000 FCFA |
| Structure mécanique | 28 500 FCFA |
| Outils et consommables | 14 000 FCFA |
| Éléments complémentaires | 10 000 FCFA |
| **Sous-total révisé** | **101 450 FCFA** |
| Réserve de sécurité 10 % | 10 145 FCFA |
| **BUDGET PRÉVISIONNEL GLOBAL** | **111 595 FCFA** |

L'ESP32 DevKit V1 est déjà possédé : il n'est pas à racheter. Sa valeur de remplacement n'est pas incluse dans les décaissements. Le budget de 111 595 FCFA inclut une provision de 5 000 FCFA pour une alimentation séparée du servo, ajoutée pour fiabiliser le montage. Le montant précédemment évoqué de 71 000 FCFA ne correspond pas à la nomenclature complète actuelle.

**À confirmer avant achat :** prix de l'ESP32-CAM, de l'OLED, de la cellule de charge, des matériaux de structure et des alimentations. Demander des devis locaux et mettre à jour les montants à partir des reçus.

# 10. ALIMENTATION ET SÉCURITÉ ÉLECTRIQUE

- L'ESP32 fonctionne avec une logique 3,3 V ; ses entrées ne doivent pas recevoir directement une tension 5 V.
- Le signal ECHO du HC-SR04 doit être adapté avant d'entrer dans l'ESP32.
- Ne pas alimenter le MG996R depuis la sortie 3,3 V de la carte.
- Prévoir une alimentation séparée et suffisamment dimensionnée pour le servo ; les servos peuvent demander un courant de pointe important.
- Relier les masses des alimentations et cartes lorsque les signaux de commande le nécessitent.
- Installer un fusible et un interrupteur général accessibles.
- Protéger les cartes contre l'eau, les déchets et les courts-circuits.
- Prévoir une commande d'arrêt et un accès manuel au couvercle.
- En cas de défaut logiciel, de perte de caméra ou de classification incertaine, garder le couvercle fermé.

# 11. LOGICIEL ET INTELLIGENCE ARTIFICIELLE

## 11.1 Répartition des tâches
- ESP32 DevKit V1 : automate principal, capteurs, servos, OLED, HX711 et logique de sécurité.
- ESP32-CAM : acquisition d'image et transmission de l'image ou du résultat de classification.
- Modèle IA : classification des objets à partir d'un jeu d'images préparé et annoté.
- LoRa : télémétrie optionnelle après validation de la carte radio et du protocole.

## 11.2 Classes de reconnaissance
Le jeu de données devra être constitué à partir des catégories de déchets que l'équipe choisira réellement. Il faudra inclure au minimum :
- les plastiques acceptés ;
- les déchets non plastiques à refuser ;
- les objets ambigus ou déformés ;
- les images dans différentes conditions d'éclairage et d'orientation.

Le système ne doit pas être présenté comme capable de reconnaître tous les plastiques tant que le modèle n'a pas été entraîné et évalué.

## 11.3 Logique de décision
- Seuil de confiance configurable.
- Classe autorisée ET confiance suffisante : autorisation.
- Classe non autorisée, confiance insuffisante, image invalide ou absence de réponse : refus.
- Temporisation maximale pour éviter de laisser le couvercle ouvert.

# 12. PLAN DE CÂBLAGE — PRINCIPES À RESPECTER

Le schéma final des broches doit être figé après vérification des références exactes des cartes et des modules. Principes :
- OLED : bus I2C SDA/SCL de l'ESP32.
- HX711 : deux lignes numériques DT et SCK.
- HC-SR04 : TRIG en sortie et ECHO en entrée avec adaptation de niveau.
- PIR/IR : sortie numérique vers une entrée ESP32.
- Servos : signal PWM depuis l'ESP32 ; alimentation externe adaptée et masse commune.
- ESP32-CAM : liaison Wi-Fi ou UART selon l'architecture logicielle retenue.
- Buzzer et LEDs : sorties avec résistances adaptées.
- LoRa : SPI et broches de commande selon le module exact retenu.

Un tableau GPIO définitif et un schéma de câblage validé sur table devront être ajoutés après choix du câblage réel. Ne pas utiliser un brochage supposé pour alimenter ou câbler le prototype.

# 13. TESTS ET CRITÈRES DE VALIDATION

| N° | Essai | Résultat attendu |
|---|---|---|
| T1 | Mise sous tension | Initialisation et message « prêt » |
| T2 | Une personne approche | Détection sans ouverture |
| T3 | Une bouteille plastique autorisée est présentée | Capture, classification et validation |
| T4 | Objet non plastique présenté | Refus et couvercle fermé |
| T5 | Image floue ou objet inconnu | Refus par défaut |
| T6 | Caméra déconnectée | Couvercle fermé et signal d'erreur |
| T7 | Validation plastique | Ouverture du couvercle |
| T8 | Dépôt du déchet | Orientation vers le compartiment programmé |
| T9 | Variation de masse | Mesure stable après calibration |
| T10 | Fin du cycle | Fermeture et retour à l'attente |

Les essais de reconnaissance devront être effectués sur un jeu de test distinct des images d'entraînement. L'équipe consignera le nombre de bonnes classifications, de faux positifs et de faux négatifs. Aucun taux de précision ne sera annoncé avant mesure réelle.

# 14. LIMITES ET RISQUES

- La caméra ne suffit pas à elle seule : un modèle IA doit être entraîné et testé.
- Les reflets, la saleté, les déformations et le faible éclairage peuvent perturber la reconnaissance.
- Le SG90 est destiné à une trappe légère ; le mécanisme doit être testé avant fabrication définitive.
- Le couple du MG996R doit être vérifié selon le poids et la géométrie du couvercle.
- Une seule cellule de charge ne mesure pas séparément le poids de deux bacs.
- Le module LoRa exact et sa fréquence doivent être déterminés avant intégration.
- Les prix sont prévisionnels et doivent être confirmés par devis.

# 15. LIVRABLES ATTENDUS

1. Cahier des charges technique et fonctionnel.
2. Nomenclature illustrée et budget détaillé.
3. Plans mécaniques cotés et photos de fabrication.
4. Schéma électronique et tableau de brochage.
5. Firmware ESP32 et ESP32-CAM.
6. Modèle IA et description du jeu de données.
7. Simulation de la partie contrôleur lorsque les composants sont pris en charge.
8. Procédure d'assemblage et de calibration.
9. Rapport d'essais avec résultats mesurés.
10. Vidéo de démonstration du prototype final.

# 16. CALENDRIER INDICATIF

| Phase | Travail |
|---|---|
| 1 | Validation du cahier des charges, choix des classes de déchets et devis |
| 2 | Plans mécaniques et achat des composants |
| 3 | Fabrication du châssis et des compartiments |
| 4 | Montage électronique sur table |
| 5 | Programmation des capteurs, servos, écran et pesée |
| 6 | Développement et test du modèle de reconnaissance |
| 7 | Intégration mécanique et électronique |
| 8 | Essais, corrections et démonstration |

# 17. CONCLUSION

PLASTIC-BOT WOURILAB associe électronique embarquée, vision artificielle, automatisme, mécanique et sensibilisation environnementale. Sa particularité est de conditionner l'ouverture à la reconnaissance du déchet, puis d'automatiser l'orientation et le suivi du poids.

Le prototype ne sera considéré comme fonctionnel qu'après validation des essais de reconnaissance, de sécurité, d'ouverture, de tri et de pesée sur le matériel réellement assemblé.

---

## SOURCES PHOTOGRAPHIQUES ET PRIX DE RÉFÉRENCE

- [DBL Électronique — catalogue de composants à Douala](https://dbl-electronique.com/) : HC-SR04 à 1 000 FCFA, SG90 à 1 500 FCFA, MG996R/MG995 à 4 000 FCFA, module LoRa Ra-01 à 3 500 FCFA, jumpers à 1 000 FCFA et plusieurs composants de prototypage.
- [E-Lab 360 — boutique électronique](https://e-lab360.net/boutique-electronique/) : références indicatives de composants et modules de pesée.
- Les images intégrées dans ce document sont des photographies de référence externes. Les liens et les prix doivent être revérifiés au moment de l'achat.
