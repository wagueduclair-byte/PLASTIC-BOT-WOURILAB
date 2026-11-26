/*
 * PLASTIC-BOT WOURILAB v2 — controleur ESP32
 * Une entrée commune, une goulotte, trois mini-bacs.
 * Le classificateur n'est PAS encore intégré : tri inhibé par défaut.
 */
#include <Arduino.h>
#include <ESP32Servo.h>

constexpr int SERVO_PIN = 18;
constexpr int BUTTON_PIN = 27;
constexpr int ANGLE_NEUTRAL = 90;
constexpr int ANGLE_ORGANIC = 35;
constexpr int ANGLE_RECYCLABLE = 90;
constexpr int ANGLE_RESIDUAL = 145;
constexpr float MIN_CONFIDENCE = 0.80f;

enum WasteCategory { UNKNOWN, ORGANIC, RECYCLABLE, RESIDUAL };
enum SystemState { IDLE, DETECTED, CLASSIFYING, RELEASE, RETURNING, SAFE_ERROR };
struct Classification { WasteCategory category; float confidence; };

Servo chute;
SystemState state = IDLE;
unsigned long started = 0;

Classification classifyWaste() {
  // TODO: appeler un modèle entraîné et validé sur des images réelles.
  // Tant que le modèle n'existe pas, ne jamais choisir une destination au hasard.
  return {UNKNOWN, 0.0f};
}
int targetAngle(WasteCategory c) {
  switch(c) {
    case ORGANIC: return ANGLE_ORGANIC;
    case RECYCLABLE: return ANGLE_RECYCLABLE;
    case RESIDUAL: return ANGLE_RESIDUAL;
    default: return ANGLE_NEUTRAL;
  }
}
void safeError(const char* msg) {
  Serial.print("SAFE_ERROR: "); Serial.println(msg);
  chute.write(ANGLE_NEUTRAL);
  state = SAFE_ERROR;
}
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  chute.setPeriodHertz(50);
  chute.attach(SERVO_PIN, 500, 2400);
  chute.write(ANGLE_NEUTRAL);
  Serial.println("PLASTIC-BOT v2 : IA non integree, tri automatique inhibe.");
}
void loop() {
  switch(state) {
    case IDLE:
      // Bouton de test uniquement; le capteur de présence ne décide pas du tri.
      if(digitalRead(BUTTON_PIN)==LOW) { delay(30); if(digitalRead(BUTTON_PIN)==LOW) state=DETECTED; }
      break;
    case DETECTED:
      Serial.println("Objet present : lancer acquisition camera.");
      state=CLASSIFYING;
      break;
    case CLASSIFYING: {
      Classification r=classifyWaste();
      if(r.category==UNKNOWN || r.confidence<MIN_CONFIDENCE) safeError("classification absente/invalide");
      else {
        chute.write(targetAngle(r.category));
        started=millis(); state=RELEASE;
      }
      break;
    }
    case RELEASE:
      if(millis()-started>700) { chute.write(ANGLE_NEUTRAL); started=millis(); state=RETURNING; }
      break;
    case RETURNING:
      if(millis()-started>500) state=IDLE;
      break;
    case SAFE_ERROR:
      // Reset après diagnostic; aucune destination aléatoire.
      delay(100);
      break;
  }
}
