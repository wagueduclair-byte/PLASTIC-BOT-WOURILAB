/*
 * Placeholder caméra — aucun modèle de classification installé.
 * Une caméra capture des images mais ne constitue pas à elle seule une IA.
 * Ajouter un modèle entraîné et évalué avant toute activation du tri.
 */
#include <Arduino.h>
void setup() {
  Serial.begin(115200);
  Serial.println("Camera placeholder: classifier absent.");
  Serial.println("Sorting remains disabled until a validated model is connected.");
}
void loop() { delay(1000); }
