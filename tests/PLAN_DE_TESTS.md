# Plan de tests — PLASTIC-BOT WOURILAB à trois entrées

| ID | Essai | Résultat attendu | Statut |
|---|---|---|---|
| T01 | Mise sous tension | Les trois clapets sont fermés | À faire |
| T02 | Présence humaine seule | Aucune entrée ne s'ouvre | À faire |
| T03 | Classification organique | Entrée organique seule s'ouvre | À faire |
| T04 | Classification recyclable | Entrée recyclable seule s'ouvre | À faire |
| T05 | Classification résiduelle | Entrée résiduelle seule s'ouvre | À faire |
| T06 | Objet inconnu / classe hors périmètre | Trois entrées fermées | À faire |
| T07 | Score de confiance sous le seuil | Trois entrées fermées | À faire |
| T08 | Caméra absente ou timeout | Trois entrées fermées, erreur signalée | À faire |
| T09 | Réponse JSON invalide | Trois entrées fermées | À faire |
| T10 | Deux demandes rapprochées | Un seul cycle à la fois | À faire |
| T11 | Fin du dépôt | Clapet concerné se referme | À faire |
| T12 | Redémarrage du contrôleur | Les trois clapets reviennent fermés | À faire |
| T13 | Pesée | Poids total stable après calibration | À faire |

La simulation Wokwi remplace la réponse du classifieur par trois boutons de test. Elle ne valide pas la reconnaissance caméra. Pour chaque essai matériel, consigner date, version firmware, conditions, observations et photo du montage. Évaluer le modèle sur des images indépendantes et consigner faux positifs et faux négatifs.
