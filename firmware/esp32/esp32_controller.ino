/*
 * PLASTIC-BOT WOURILAB — ESP32 DevKit V1 (firmware matériel)
 * Chaîne : présence humaine -> objet présenté -> décision ESP32-CAM
 * -> ouverture du couvercle -> orientation -> pesée -> affichage.
 *
 * SECURITE : PIR seul n'ouvre JAMAIS le couvercle.
 * Toute erreur caméra / Wi-Fi / JSON = REFUS.
 *
 * Bibliothèques : ESP32Servo, Adafruit GFX, Adafruit SSD1306,
 * HX711 (Bogdan Necula), ArduinoJson.
 * Adapter les broches et CAMERA_DECISION_URL au montage réel.
 */
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HX711.h"
#include <ESP32Servo.h>

// ---------- WIFI / CAMERA ----------
const char* WIFI_SSID = "VOTRE_WIFI";
const char* WIFI_PASSWORD = "VOTRE_MOT_DE_PASSE";
const char* CAMERA_DECISION_URL = "http://192.168.1.50/decision";

// ---------- BROCHAGE ESP32 DEVKIT V1 ----------
constexpr uint8_t PIN_PIR = 27;
constexpr uint8_t PIN_TRIG = 25;
constexpr uint8_t PIN_ECHO = 26;       // Diviseur 5V -> 3,3V obligatoire en réel
constexpr uint8_t PIN_SERVO_COUVERCLE = 18;
constexpr uint8_t PIN_SERVO_TRIEUR = 19;
constexpr uint8_t PIN_HX711_DOUT = 32;
constexpr uint8_t PIN_HX711_SCK = 33;
constexpr uint8_t PIN_LED_OK = 2;
constexpr uint8_t PIN_LED_REFUS = 4;
constexpr uint8_t PIN_BUZZER = 14;
constexpr uint8_t PIN_OLED_SDA = 21;
constexpr uint8_t PIN_OLED_SCL = 22;

// ---------- REGLAGES MECANIQUES ----------
constexpr float DISTANCE_OBJET_CM = 18.0f;
constexpr int COUVERCLE_FERME = 0;
constexpr int COUVERCLE_OUVERT = 85;
constexpr int TRIEUR_ATTENTE = 90;
constexpr int TRIEUR_COMPARTIMENT_PLASTIQUE = 35;
constexpr unsigned long TEMPS_OUVERTURE_MS = 2200;
constexpr unsigned long ANTI_REPETITION_MS = 3500;
constexpr unsigned long DELAI_DECISION_CAMERA_MS = 5000;

// Facteur provisoire : calibrer avec une masse connue sur le vrai montage.
constexpr float FACTEUR_CALIBRATION = 420.0f;

Adafruit_SSD1306 oled(128, 64, &Wire, -1);
HX711 balance;
Servo servoCouvercle;
Servo servoTrieur;

unsigned long dernierCycle = 0;
bool ecranDisponible = false;
bool balanceDisponible = false;

enum Etat { ATTENTE, PRESENCE, OBJET, ANALYSE, ACCEPTE, REFUSE, PANNE };
Etat etat = ATTENTE;

void afficher(const char* ligne1, const char* ligne2 = "") {
  if (!ecranDisponible) return;
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 8);
  oled.println("PLASTIC-BOT WOURILAB");
  oled.drawLine(0, 20, 127, 20, SSD1306_WHITE);
  oled.setCursor(0, 29);
  oled.println(ligne1);
  oled.setCursor(0, 44);
  oled.println(ligne2);
  oled.display();
}

float mesurerDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(3);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  const unsigned long duree = pulseIn(PIN_ECHO, HIGH, 25000);
  if (duree == 0) return -1.0f;
  return duree * 0.0343f / 2.0f;
}

// Retourne true UNIQUEMENT si la caméra renvoie un JSON valide accepted=true.
bool objetEstPlastique() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[SECURITE] Wi-Fi absent : refus.");
    return false;
  }

  HTTPClient http;
  http.setConnectTimeout(2000);
  http.setTimeout(DELAI_DECISION_CAMERA_MS);
  if (!http.begin(CAMERA_DECISION_URL)) return false;

  const int codeHTTP = http.GET();
  if (codeHTTP != HTTP_CODE_OK) {
    Serial.printf("[CAMERA] HTTP %d : refus.\n", codeHTTP);
    http.end();
    return false;
  }

  const String reponse = http.getString();
  http.end();

  JsonDocument doc;
  const DeserializationError erreur = deserializeJson(doc, reponse);
  if (erreur || !doc["accepted"].is<bool>()) {
    Serial.println("[CAMERA] JSON invalide : refus.");
    return false;
  }

  const bool accepte = doc["accepted"].as<bool>();
  const float confiance = doc["confidence"] | 0.0f;
  Serial.printf("[CAMERA] accepted=%s confidence=%.2f\n",
                accepte ? "true" : "false", confiance);

  // Seuil minimal à ajuster après entraînement et validation du modèle.
  return accepte && confiance >= 0.80f;
}

void signalerRefus(const char* raison) {
  etat = REFUSE;
  Serial.printf("[REFUS] %s\n", raison);
  afficher("OBJET REFUSE", raison);
  digitalWrite(PIN_LED_REFUS, HIGH);
  tone(PIN_BUZZER, 650, 350);
  delay(500);
  digitalWrite(PIN_LED_REFUS, LOW);
  servoCouvercle.write(COUVERCLE_FERME);
  servoTrieur.write(TRIEUR_ATTENTE);
}

void mesurerEtAfficherPoids() {
  if (!balanceDisponible || !balance.is_ready()) {
    afficher("PLASTIQUE ACCEPTE", "Balance indisponible");
    Serial.println("[POIDS] HX711 indisponible.");
    return;
  }
  const float poidsKg = balance.get_units(5);
  const float poidsGrammes = poidsKg * 1000.0f;
  char texte[24];
  snprintf(texte, sizeof(texte), "Poids: %.0f g", poidsGrammes);
  afficher("DEPOT ENREGISTRE", texte);
  Serial.println(texte);
}

void cycleAccepte() {
  etat = ACCEPTE;
  Serial.println("[ACCEPTE] Plastique confirme.");
  digitalWrite(PIN_LED_OK, HIGH);
  tone(PIN_BUZZER, 1400, 150);
  afficher("PLASTIQUE ACCEPTE", "Ouverture...");
  servoCouvercle.write(COUVERCLE_OUVERT);
  delay(700);
  servoTrieur.write(TRIEUR_COMPARTIMENT_PLASTIQUE);
  delay(TEMPS_OUVERTURE_MS);
  servoTrieur.write(TRIEUR_ATTENTE);
  servoCouvercle.write(COUVERCLE_FERME);
  delay(500);
  digitalWrite(PIN_LED_OK, LOW);
  mesurerEtAfficherPoids();
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_LED_OK, OUTPUT);
  pinMode(PIN_LED_REFUS, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_TRIG, LOW);
  digitalWrite(PIN_LED_OK, LOW);
  digitalWrite(PIN_LED_REFUS, LOW);

  Wire.begin(PIN_OLED_SDA, PIN_OLED_SCL);
  ecranDisponible = oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  afficher("Initialisation...", "Couvercle ferme");

  servoCouvercle.setPeriodHertz(50);
  servoTrieur.setPeriodHertz(50);
  servoCouvercle.attach(PIN_SERVO_COUVERCLE, 500, 2400);
  servoTrieur.attach(PIN_SERVO_TRIEUR, 500, 2400);
  servoCouvercle.write(COUVERCLE_FERME);
  servoTrieur.write(TRIEUR_ATTENTE);

  balance.begin(PIN_HX711_DOUT, PIN_HX711_SCK);
  balance.set_scale(FACTEUR_CALIBRATION);
  if (balance.wait_ready_timeout(1500)) {
    balance.tare();
    balanceDisponible = true;
  } else {
    Serial.println("[AVERTISSEMENT] HX711 non detecte.");
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  const unsigned long debut = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - debut < 10000) {
    delay(250);
    Serial.print(".");
  }
  Serial.println();
  Serial.println(WiFi.status() == WL_CONNECTED
    ? "[WIFI] Connecte." : "[WIFI] Indisponible : refus securitaire.");

  afficher("Pret a recevoir", "Presentez un objet");
}

void loop() {
  // 1. Présence humaine : activation de la zone seulement, jamais d'ouverture.
  if (digitalRead(PIN_PIR) == HIGH) {
    if (etat == ATTENTE) {
      etat = PRESENCE;
      afficher("Utilisateur detecte", "Presentez un objet");
      Serial.println("[PRESENCE] Zone active; couvercle toujours ferme.");
    }
  } else if (etat == PRESENCE) {
    etat = ATTENTE;
    afficher("En attente", "Approchez-vous");
  }

  // 2. Sans présence humaine, aucun cycle de tri n'est lancé.
  if (etat != PRESENCE || millis() - dernierCycle < ANTI_REPETITION_MS) {
    delay(80);
    return;
  }

  // 3. Détection de l'objet présenté dans la zone.
  const float distance = mesurerDistanceCm();
  if (distance <= 0 || distance > DISTANCE_OBJET_CM) {
    delay(100);
    return;
  }

  dernierCycle = millis();
  etat = OBJET;
  afficher("Objet detecte", "Analyse en cours...");
  Serial.printf("[OBJET] Distance: %.1f cm\n", distance);

  // 4. Classification IA distante. Toute panne ou incertitude = refus.
  etat = ANALYSE;
  const bool plastique = objetEstPlastique();

  // 5. Seul un résultat positif et suffisamment fiable autorise l'ouverture.
  if (plastique) cycleAccepte();
  else signalerRefus("Non plastique / doute");

  // 6. Retour à l'état sûr.
  servoCouvercle.write(COUVERCLE_FERME);
  servoTrieur.write(TRIEUR_ATTENTE);
  delay(1000);
  etat = ATTENTE;
  afficher("En attente", "Presentez un objet");
}
