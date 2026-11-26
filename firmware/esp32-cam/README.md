# Firmware ESP32-CAM — scanner à trois catégories

La caméra capture l'image présentée et expose l'API HTTP GET /decision.

Réponse attendue : un objet JSON contenant category et confidence. Les catégories autorisées sont organic, recyclable et residual. Une valeur inconnue ou une confiance inférieure à 0,80 doit produire unknown.

**État actuel :** le code initialise la caméra et expose l'API, mais le modèle de reconnaissance n'est pas encore entraîné ni intégré. La fonction classifyWaste() retourne volontairement unknown/0.0. Par conséquent, aucune entrée ne s'ouvrira tant que le modèle réel n'aura pas été intégré et validé.

Étapes : renseigner le Wi-Fi, téléverser sur une ESP32-CAM AI Thinker avec alimentation stable, relever son IP, la renseigner dans le contrôleur ESP32, constituer et annoter les images des trois classes, entraîner le modèle et évaluer ses résultats sur des images indépendantes.
