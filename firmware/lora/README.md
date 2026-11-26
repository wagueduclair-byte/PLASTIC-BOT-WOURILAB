# Module LoRa — intégration à confirmer

Le projet prévoit une liaison LoRa pour transmettre l'état de la poubelle (niveau de remplissage, compteur d'acceptations, défauts).

**La référence exacte de la carte LoRa DevKit n'est pas encore confirmée.** Plusieurs cartes utilisent des puces, broches, tensions et bibliothèques différentes. Aucun brochage ni firmware matériel définitif n'est donc inventé ici.

Dès que la référence inscrite sur la carte est connue :
1. relever la puce radio (SX1276/SX1278 ou autre);
2. relever la fréquence autorisée localement et l'antenne adaptée;
3. vérifier les broches SPI, RESET et DIO;
4. choisir la bibliothèque correspondant à la carte;
5. tester l'envoi/réception avec deux modules compatibles.

Ne jamais connecter un module radio sur la base d'un brochage supposé.
