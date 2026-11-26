# PLASTIC-BOT-WOURILAB

## Documents principaux
- [CAHIER DES CHARGES](CAHIER_DES_CHARGES.md) — version complète à la racine.
- [Budget prévisionnel](docs/BUDGET.md)
- [Cahier des charges détaillé](docs/CAHIER_DES_CHARGES.md)


Prototype de poubelle intelligente qui n'accepte que les déchets plastiques.

## Principe de fonctionnement
1. Le capteur de présence détecte un utilisateur ou un objet à proximité.
2. Le système de vision (ESP32-CAM) analyse l'objet présenté.
3. Si l'objet est reconnu comme plastique accepté, le servomoteur ouvre le couvercle.
4. Si l'objet est refusé ou si la décision est incertaine, le couvercle reste fermé et un signal sonore/visuel informe l'utilisateur.

> **Sécurité de conception :** la détection de présence seule ne doit jamais déclencher l'ouverture. L'ouverture dépend de la validation du plastique. En cas de panne ou de classification incertaine, le système reste fermé.

## Matériel de base
- ESP32 DevKit V1 (déjà possédé par le porteur du projet)
- ESP32-CAM AI Thinker (à acheter / à confirmer)
- Capteur ultrason HC-SR04 (présence/proximité)
- Capteur IR (détection complémentaire)
- Servomoteur SG90 (maquette légère; couple à vérifier pour le couvercle réel)
- OLED I2C, buzzer, LEDs
- Alimentation 5 V régulée et convertisseur adaptés

La carte LoRa DevKit est prévue pour la télémétrie distante, mais sa référence exacte doit être confirmée avant de figer le câblage et le firmware LoRa.

## Arborescence
- `docs/` : cahier des charges, architecture, budget et validation
- `hardware/` : nomenclature, câblage et notes mécaniques
- `firmware/esp32/` : contrôleur principal
- `firmware/esp32-cam/` : serveur de vision / interface de classification
- `firmware/lora/` : squelette de télémétrie à adapter à la carte choisie
- `simulation/wokwi/` : démarrage de simulation
- `tests/` : plan de tests
- `images/` : emplacement des photos réelles des composants

## Démarrer
1. Ouvrir `firmware/esp32/` dans Arduino IDE ou PlatformIO.
2. Installer les bibliothèques indiquées dans le fichier source.
3. Renseigner l'adresse IP de l'ESP32-CAM dans le contrôleur.
4. Simuler d'abord les capteurs et le servo dans Wokwi. La caméra et LoRa nécessitent des tests matériels séparés.

## État du prototype
Ce dépôt contient une base technique initiale. La reconnaissance fiable du plastique doit être entraînée et validée sur des images représentatives avant toute utilisation réelle. Le firmware ESP32-CAM fourni ici est un point d'intégration, pas un modèle IA déjà entraîné.

## Budget
Le budget détaillé est dans [docs/BUDGET.md](docs/BUDGET.md). Le total provisoire antérieur était de **101 145 FCFA** avec réserve, tandis que **71 000 FCFA** a été évoqué plus tard sans détail cohérent. Le montant final reste donc à confirmer avec les prix locaux et les composants effectivement retenus.
