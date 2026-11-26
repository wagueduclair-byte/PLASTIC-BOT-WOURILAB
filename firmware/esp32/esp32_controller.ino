/*
  PLASTIC-BOT WOURILAB — ESP32 DevKit V1
  Prototype de commande fail-safe.
  Dépendance : ESP32 Arduino core.
  La décision de classification est fournie par l'ESP32-CAM via HTTP.
  IMPORTANT : ajuster broches, IP caméra et alimentation avant montage.
*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";
const char* CAMERA_DECISION_URL = "http://192.168.1.50/decision";

constexpr int TRIG_PIN = 5;
constexpr int ECHO_PIN = 18;  // utiliser un diviseur de tension sur ECHO 5 V
constexpr int SERVO_PIN = 19;
constexpr int LED_OK = 2;
constexpr int LED_NO = 4;
constexpr int BUZZER_PIN = 23;

constexpr float DETECTION_DISTANCE_CM = 18.0;
constexpr int CLOSED_ANGLE = 0;
constexpr int OPEN_ANGLE = 85;
constexpr unsigned long OPEN_DURATION_MS = 2500;
constexpr unsigned long COOLDOWN_MS = 4000;

Servo lid;
unsigned long lastCycle = 0;
bool busy = false;

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1.0;
  return duration * 0.0343f / 2.0f;
}

bool cameraAcceptsPlastic() {
  if (WiFi.status() != WL_CONNECTED) return false;
  HTTPClient http;
  http.setTimeout(3500);
  if (!http.begin(CAMERA_DECISION_URL)) return false;
  int code = http.GET();
  String body = (code == HTTP_CODE_OK) ? http.getString() : "";
  http.end();
  body.trim();
  body.toLowerCase();
  // L'endpoint doit renvoyer exactement {"accepted":true}
  return code == HTTP_CODE_OK && body.indexOf("\"accepted\":true") >= 0;
}

void signalReject() {
  digitalWrite(LED_NO, HIGH);
  tone(BUZZER_PIN, 700, 250);
  delay(300);
  digitalWrite(LED_NO, LOW);
}

void runAcceptedCycle() {
  digitalWrite(LED_OK, HIGH);
  tone(BUZZER_PIN, 1400, 120);
  lid.write(OPEN_ANGLE);
  delay(OPEN_DURATION_MS);
  lid.write(CLOSED_ANGLE);
  digitalWrite(LED_OK, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_OK, OUTPUT);
  pinMode(LED_NO, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  lid.setPeriodHertz(50);
  lid.attach(SERVO_PIN, 500, 2400);
  lid.write(CLOSED_ANGLE);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connexion Wi-Fi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 12000) {
    delay(300);
    Serial.print(".");
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? "\nWi-Fi connecté" : "\nWi-Fi indisponible : refus par défaut");
}

void loop() {
  if (busy || millis() - lastCycle < COOLDOWN_MS) {
    delay(50);
    return;
  }

  float distance = readDistanceCm();
  if (distance > 0 && distance <= DETECTION_DISTANCE_CM) {
    busy = true;
    lastCycle = millis();
    delay(250); // stabilisation de la présentation
    bool accepted = cameraAcceptsPlastic();
    if (accepted) runAcceptedCycle();
    else signalReject();
    lid.write(CLOSED_ANGLE);
    busy = false;
  }
  delay(80);
}
