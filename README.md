# PLASTIC-BOT WOURILAB
### Poubelle intelligente à tri automatique — prototype étudiant

PLASTIC-BOT reçoit les déchets par **une seule ouverture**, les observe dans une zone de reconnaissance, puis les dirige vers le compartiment intérieur correspondant grâce à une goulotte orientable.

> **État réel :** conception et préparation du prototype. Le classificateur IA n'est pas encore entraîné ni validé sur des déchets réels. La démonstration logicielle simule les catégories.

## Architecture retenue
- Un grand bac extérieur unique et une seule entrée.
- Une caméra dans une zone de reconnaissance.
- Une goulotte orientable.
- Trois mini-bacs intérieurs proposés : organiques, recyclables, résiduels.
- Une ESP32 commande les capteurs et les servomoteurs.

## Parcours d'un déchet
1. Dépôt par l'entrée commune.
2. Détection de présence (réveil uniquement, pas de décision de tri).
3. Capture d'image et classification.
4. Vérification de la catégorie et du niveau de confiance.
5. Orientation de la goulotte vers le mini-bac cible.
6. Libération du déchet et retour en position neutre.

## Documents
- [Cahier des charges](CAHIER_DES_CHARGES.md)
- [Composants](hardware/BOM.md)
- [Budget prévisionnel](docs/BUDGET.md)
- [Architecture et câblage](docs/ARCHITECTURE.md)
- [Firmware ESP32](firmware/esp32/esp32_controller.ino)
- [Simulation Wokwi](simulation/wokwi/sketch.ino)
- [Démonstration web](site/index.html)
- [Plan de tests](tests/PLAN_DE_TESTS.md)

## Matériel déclaré
La carte ESP32 DevKit V1 est disponible selon les informations communiquées. L'ESP32-CAM n'est pas encore disponible. Vérifier les autres composants avant achat.

## Sécurité et limites
- Ne jamais alimenter les servos par la sortie 3,3 V de l'ESP32.
- Prévoir une alimentation moteur adaptée et une masse commune.
- Adapter le signal ECHO 5 V du HC-SR04 avant une entrée ESP32.
- Une simulation de boutons n'est pas une reconnaissance IA.
- En cas de résultat inconnu ou de panne, le tri doit rester inhibé.

Projet éducatif — version 2.0, novembre 2026.
