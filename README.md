# PLASTIC-BOT WOURILAB
### Poubelle intelligente à trois entrées avec scanner et tri automatique

Le système identifie un déchet avant le dépôt et n'ouvre que l'entrée correspondant à sa catégorie.

## Trois catégories
1. **Organiques / biodégradables** : restes alimentaires, épluchures, feuilles.
2. **Recyclables** : bouteilles et emballages plastiques, papier, carton et canettes.
3. **Résiduels / non recyclables** : déchets souillés ou non valorisables dans les deux autres filières.

## Fonctionnement
1. Le PIR détecte l'utilisateur, sans commander d'ouverture.
2. L'utilisateur présente un objet dans la zone de scan.
3. La caméra capture l'image.
4. Le modèle IA retourne organic, recyclable, residual ou unknown.
5. Le contrôleur valide la catégorie et le score.
6. Un seul des trois servomoteurs ouvre l'entrée correspondante.
7. En cas de doute ou de panne, les trois entrées restent fermées.

## Documents
- [Cahier des charges](CAHIER_DES_CHARGES.md)
- [Budget prévisionnel](docs/BUDGET.md)
- [Nomenclature matérielle](hardware/BOM.md)
- [Photos de référence](PHOTOS_COMPOSANTS.md)
- [Firmware ESP32](firmware/esp32/README.md)
- [Firmware ESP32-CAM](firmware/esp32-cam/README.md)
- [Simulation Wokwi](simulation/wokwi/README.md)
- [Démonstration interactive](simulation/demo/README.md)
- [Plan de tests](tests/PLAN_DE_TESTS.md)

## État réel
L'ESP32 DevKit V1 est déjà possédé. Le module ESP32-CAM, les trois servomoteurs et les matériaux sont à vérifier. Le modèle IA à trois classes doit être entraîné, intégré et évalué. Une simulation par boutons ne constitue pas une reconnaissance réelle.

## Budget
Le budget de la version trois entrées est provisoire et doit être confirmé par des devis locaux. Voir docs/BUDGET.md.

## Démonstration et publication
La démonstration web est une animation pédagogique. Wokwi teste uniquement les composants pris en charge par le simulateur. Pour GitHub Pages, ouvrir Settings → Pages → Build and deployment et choisir GitHub Actions. Le site n'est publié qu'après réussite du workflow.
