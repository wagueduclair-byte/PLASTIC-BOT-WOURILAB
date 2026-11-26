/*
 * PLASTIC-BOT WOURILAB — ESP32, version trois entrées.
 * API caméra attendue : {"category":"organic","confidence":0.93}
 * Catégories autorisées : organic, recyclable, residual.
 * Toute erreur / unknown / confiance faible = trois clapets fermés.
 *
 * Bibliothèques : ESP32Servo, Adafruit GFX, Adafruit SSD1306,
 * HX711 (optionnel), ArduinoJson.
 */
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HX711.h"
#include <ESP32Servo.h>

const char* WIFI_SSID="VOTRE_WIFI";
const char* WIFI_PASSWORD="VOTRE_MOT_DE_PASSE";
const char* CAMERA_DECISION_URL="http://192.168.1.50/decision";

constexpr uint8_t PIR=27, TRIG=25, ECHO=26;
constexpr uint8_t SERVO_ORGANIC=18, SERVO_RECYCLABLE=19, SERVO_RESIDUAL=23;
constexpr uint8_t HX_DT=32, HX_SCK=33, LED_OK=2, LED_ERROR=4, BUZZER=14;
constexpr float MAX_DISTANCE_CM=18.0f, MIN_CONFIDENCE=0.80f, SCALE_FACTOR=420.0f;
constexpr int CLOSED=0, OPEN=85;
constexpr unsigned long OPEN_MS=1800, GUARD_MS=2500;

Adafruit_SSD1306 oled(128,64,&Wire,-1);
HX711 scale;
Servo servoOrganic, servoRecyclable, servoResidual;
bool oledOK=false, scaleOK=false, busy=false;
unsigned long lastCycle=0;

void closeAll() {
  servoOrganic.write(CLOSED);
  servoRecyclable.write(CLOSED);
  servoResidual.write(CLOSED);
}
void screen(const char* a,const char* b="") {
  if(!oledOK) return;
  oled.clearDisplay(); oled.setTextSize(1); oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0,5); oled.println("PLASTIC-BOT WOURILAB");
  oled.drawLine(0,17,127,17,SSD1306_WHITE);
  oled.setCursor(0,27); oled.println(a); oled.setCursor(0,43); oled.println(b);
  oled.display();
}
float distanceCm() {
  digitalWrite(TRIG,LOW); delayMicroseconds(3);
  digitalWrite(TRIG,HIGH); delayMicroseconds(10); digitalWrite(TRIG,LOW);
  unsigned long t=pulseIn(ECHO,HIGH,25000);
  return t ? t*0.0343f/2.0f : -1.0f;
}
bool getCategory(String &category) {
  if(WiFi.status()!=WL_CONNECTED) return false;
  HTTPClient http; http.setConnectTimeout(2000); http.setTimeout(5000);
  if(!http.begin(CAMERA_DECISION_URL)) return false;
  int status=http.GET();
  if(status!=HTTP_CODE_OK) { http.end(); return false; }
  String body=http.getString(); http.end();
  JsonDocument doc;
  if(deserializeJson(doc,body)) return false;
  if(!doc["category"].is<const char*>()) return false;
  if(!doc["confidence"].is<float>() && !doc["confidence"].is<double>()) return false;
  category=String(doc["category"].as<const char*>());
  float confidence=doc["confidence"].as<float>();
  return confidence>=MIN_CONFIDENCE &&
    (category=="organic" || category=="recyclable" || category=="residual");
}
void rejectObject() {
  closeAll(); digitalWrite(LED_ERROR,HIGH); tone(BUZZER,600,350);
  screen("DECHET NON IDENTIFIE","Reessayez");
  delay(600); digitalWrite(LED_ERROR,LOW);
}
void openCategory(const String &category) {
  closeAll();
  if(category=="organic") {
    screen("DECHET ORGANIQUE","Entree 1");
    servoOrganic.write(OPEN);
  } else if(category=="recyclable") {
    screen("DECHET RECYCLABLE","Entree 2");
    servoRecyclable.write(OPEN);
  } else if(category=="residual") {
    screen("DECHET RESIDUEL","Entree 3");
    servoResidual.write(OPEN);
  } else { rejectObject(); return; }

  digitalWrite(LED_OK,HIGH); tone(BUZZER,1300,150);
  delay(OPEN_MS); closeAll(); digitalWrite(LED_OK,LOW);
  if(scaleOK && scale.is_ready()) {
    char line[24]; snprintf(line,sizeof(line),"Poids total %.0f g",scale.get_units(3)*1000.0f);
    screen("DEPOT ENREGISTRE",line); Serial.println(line);
  } else screen("DEPOT ENREGISTRE","Cycle termine");
}
void setup() {
  Serial.begin(115200);
  pinMode(PIR,INPUT); pinMode(TRIG,OUTPUT); pinMode(ECHO,INPUT);
  pinMode(LED_OK,OUTPUT); pinMode(LED_ERROR,OUTPUT); pinMode(BUZZER,OUTPUT);
  Wire.begin(21,22); oledOK=oled.begin(SSD1306_SWITCHCAPVCC,0x3C);
  servoOrganic.setPeriodHertz(50); servoRecyclable.setPeriodHertz(50); servoResidual.setPeriodHertz(50);
  servoOrganic.attach(SERVO_ORGANIC,500,2400);
  servoRecyclable.attach(SERVO_RECYCLABLE,500,2400);
  servoResidual.attach(SERVO_RESIDUAL,500,2400);
  closeAll();
  scale.begin(HX_DT,HX_SCK); scale.set_scale(SCALE_FACTOR);
  if(scale.wait_ready_timeout(1000)) { scale.tare(); scaleOK=true; }
  WiFi.mode(WIFI_STA); WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  unsigned long start=millis();
  while(WiFi.status()!=WL_CONNECTED && millis()-start<10000) delay(250);
  closeAll(); screen("SYSTEME PRET","Presentez un dechet");
}
void loop() {
  // PIR = activation de l'interface seulement ; aucun servo n'est commandé ici.
  if(digitalRead(PIR)==LOW) { screen("EN ATTENTE","Approchez-vous"); delay(100); return; }
  screen("PRESENTEZ UN OBJET","Scan en attente");
  float d=distanceCm();
  if(d<=0 || d>MAX_DISTANCE_CM || millis()-lastCycle<GUARD_MS || busy) { delay(100); return; }

  busy=true; lastCycle=millis(); closeAll();
  screen("ANALYSE EN COURS","Scanner / camera");
  String category="";
  if(getCategory(category)) openCategory(category);
  else rejectObject();
  closeAll(); delay(700); busy=false;
}
