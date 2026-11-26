/*
 * PLASTIC-BOT WOURILAB — SIMULATION WOKWI
 * La caméra IA réelle est remplacée ici par le bouton vert
 * "SIMULER PLASTIQUE". Appuyer dessus pendant la fenêtre d'analyse
 * pour simuler une réponse positive de l'ESP32-CAM.
 *
 * Le PIR ne fait qu'activer la zone. Il n'ouvre jamais le couvercle.
 * Le HC-SR04 détecte l'objet. Sans bouton vert = refus.
 *
 * Composants simulés : PIR, HC-SR04, 2 servos, OLED SSD1306,
 * HX711 5 kg, LED verte/rouge, buzzer, bouton de test IA.
 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HX711.h"
#include <ESP32Servo.h>

constexpr int PIR=27, TRIG=25, ECHO=26;
constexpr int SERVO_LID=18, SERVO_SORT=19;
constexpr int HX_DOUT=32, HX_SCK=33;
constexpr int LED_OK=2, LED_NO=4, BUZZER=14;
constexpr int BTN_AI_PLASTIC=13;
constexpr float MAX_DISTANCE=18.0;
constexpr float SCALE_FACTOR=420.0; // Facteur de démonstration Wokwi

Adafruit_SSD1306 display(128,64,&Wire,-1);
HX711 scale;
Servo lidServo, sorterServo;
bool oledOK=false, scaleOK=false;
unsigned long lastCycle=0;

void screen(const char* a,const char* b="") {
  if(!oledOK) return;
  display.clearDisplay();
  display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,5); display.println("PLASTIC-BOT WOURILAB");
  display.drawLine(0,17,127,17,SSD1306_WHITE);
  display.setCursor(0,27); display.println(a);
  display.setCursor(0,43); display.println(b);
  display.display();
}
float distanceCm() {
  digitalWrite(TRIG,LOW); delayMicroseconds(3);
  digitalWrite(TRIG,HIGH); delayMicroseconds(10); digitalWrite(TRIG,LOW);
  unsigned long t=pulseIn(ECHO,HIGH,25000);
  return t ? t*0.0343f/2.0f : -1.0f;
}
bool simulateCameraDecision() {
  screen("Analyse IA simulee","Bouton vert = plastique");
  Serial.println("SIMULATION: appuyer sur le bouton vert pour accepter.");
  unsigned long start=millis();
  while(millis()-start<4000) {
    if(digitalRead(BTN_AI_PLASTIC)==LOW) {
      while(digitalRead(BTN_AI_PLASTIC)==LOW) delay(10);
      return true;
    }
    delay(10);
  }
  return false; // Refus par défaut si aucune validation explicite.
}
void rejectObject() {
  screen("OBJET REFUSE","Couvercle reste ferme");
  digitalWrite(LED_NO,HIGH); tone(BUZZER,650,300);
  delay(450); digitalWrite(LED_NO,LOW);
  lidServo.write(0); sorterServo.write(90);
}
void acceptObject() {
  screen("PLASTIQUE ACCEPTE","Ouverture du couvercle");
  digitalWrite(LED_OK,HIGH); tone(BUZZER,1400,120);
  lidServo.write(85); delay(600);
  sorterServo.write(35); delay(1800);
  sorterServo.write(90); lidServo.write(0);
  digitalWrite(LED_OK,LOW);
  if(scaleOK && scale.is_ready()) {
    float grams=scale.get_units(3)*1000.0f;
    char line[24]; snprintf(line,sizeof(line),"Poids: %.0f g",grams);
    screen("Depot enregistre",line);
    Serial.println(line);
  } else screen("Depot enregistre","HX711 indisponible");
  delay(1200);
}
void setup() {
  Serial.begin(115200);
  pinMode(PIR,INPUT); pinMode(TRIG,OUTPUT); pinMode(ECHO,INPUT);
  pinMode(LED_OK,OUTPUT); pinMode(LED_NO,OUTPUT);
  pinMode(BUZZER,OUTPUT); pinMode(BTN_AI_PLASTIC,INPUT_PULLUP);
  Wire.begin(21,22);
  oledOK=display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  lidServo.setPeriodHertz(50); sorterServo.setPeriodHertz(50);
  lidServo.attach(SERVO_LID,500,2400);
  sorterServo.attach(SERVO_SORT,500,2400);
  lidServo.write(0); sorterServo.write(90);
  scale.begin(HX_DOUT,HX_SCK);
  scale.set_scale(SCALE_FACTOR);
  if(scale.wait_ready_timeout(1000)) { scale.tare(); scaleOK=true; }
  screen("Systeme pret","Attente utilisateur");
  Serial.println("WOURILAB: PIR -> objet -> bouton IA -> tri -> poids");
}
void loop() {
  // Le PIR détecte seulement l'utilisateur; aucun mouvement de servo ici.
  if(digitalRead(PIR)==LOW) {
    screen("En attente","Approchez-vous");
    delay(100); return;
  }
  screen("Utilisateur detecte","Presentez un objet");
  float d=distanceCm();
  if(d<=0 || d>MAX_DISTANCE || millis()-lastCycle<3000) {
    delay(100); return;
  }
  lastCycle=millis();
  Serial.printf("Objet detecte a %.1f cm\n",d);
  if(simulateCameraDecision()) acceptObject();
  else rejectObject();
  lidServo.write(0); sorterServo.write(90);
  delay(700);
}
