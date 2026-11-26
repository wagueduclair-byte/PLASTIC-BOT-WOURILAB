/*
 * PLASTIC-BOT WOURILAB — Wokwi, simulation logique 3 entrées.
 * Boutons de test = réponses simulées du classifieur, PAS une caméra IA.
 * PIR seul ne commande jamais l'ouverture.
 * Bouton organique GPIO13, recyclable GPIO16, résiduel GPIO17.
 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
#include "HX711.h"

constexpr int PIR=27, TRIG=25, ECHO=26;
constexpr int SERVO_ORGANIC=18, SERVO_RECYCLABLE=19, SERVO_RESIDUAL=23;
constexpr int BTN_ORGANIC=13, BTN_RECYCLABLE=16, BTN_RESIDUAL=17;
constexpr int HX_DT=32, HX_SCK=33;
constexpr int LED_OK=2, LED_ERROR=4, BUZZER=14;
constexpr float MAX_DISTANCE=18.0f, SCALE_FACTOR=420.0f;

Adafruit_SSD1306 display(128,64,&Wire,-1);
Servo organicServo,recyclableServo,residualServo;
HX711 scale;
bool oledOK=false,scaleOK=false;

void closeAll(){organicServo.write(0);recyclableServo.write(0);residualServo.write(0);}
void screen(const char* a,const char* b=""){
  if(!oledOK)return;
  display.clearDisplay();display.setTextSize(1);display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,5);display.println("PLASTIC-BOT WOURILAB");
  display.drawLine(0,17,127,17,SSD1306_WHITE);
  display.setCursor(0,27);display.println(a);display.setCursor(0,43);display.println(b);display.display();
}
float distanceCm(){
  digitalWrite(TRIG,LOW);delayMicroseconds(3);digitalWrite(TRIG,HIGH);delayMicroseconds(10);digitalWrite(TRIG,LOW);
  unsigned long t=pulseIn(ECHO,HIGH,25000);return t?t*0.0343f/2.0f:-1.0f;
}
void reject(){
  closeAll();digitalWrite(LED_ERROR,HIGH);tone(BUZZER,600,250);
  screen("DECHET NON IDENTIFIE","Aucune entree ouverte");delay(400);digitalWrite(LED_ERROR,LOW);
}
void acceptCategory(int category){
  closeAll();
  if(category==1){screen("ORGANIQUE","Entree 1 ouverte");organicServo.write(85);}
  else if(category==2){screen("RECYCLABLE","Entree 2 ouverte");recyclableServo.write(85);}
  else if(category==3){screen("RESIDUEL","Entree 3 ouverte");residualServo.write(85);}
  else {reject();return;}
  digitalWrite(LED_OK,HIGH);tone(BUZZER,1300,120);delay(1600);closeAll();digitalWrite(LED_OK,LOW);
  if(scaleOK&&scale.is_ready()){
    char msg[24];snprintf(msg,sizeof(msg),"Poids total %.0f g",scale.get_units(3)*1000.0f);
    screen("DEPOT SIMULE",msg);
  }else screen("DEPOT SIMULE","Cycle termine");
}
void setup(){
  Serial.begin(115200);
  pinMode(PIR,INPUT);pinMode(TRIG,OUTPUT);pinMode(ECHO,INPUT);
  pinMode(BTN_ORGANIC,INPUT_PULLUP);pinMode(BTN_RECYCLABLE,INPUT_PULLUP);pinMode(BTN_RESIDUAL,INPUT_PULLUP);
  pinMode(LED_OK,OUTPUT);pinMode(LED_ERROR,OUTPUT);pinMode(BUZZER,OUTPUT);
  Wire.begin(21,22);oledOK=display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  organicServo.attach(SERVO_ORGANIC);recyclableServo.attach(SERVO_RECYCLABLE);residualServo.attach(SERVO_RESIDUAL);closeAll();
  scale.begin(HX_DT,HX_SCK);scale.set_scale(SCALE_FACTOR);
  if(scale.wait_ready_timeout(1000)){scale.tare();scaleOK=true;}
  screen("SYSTEME PRET","Scan simule par boutons");
  Serial.println("WOKWI: boutons = reponses IA simulees, aucune vraie reconnaissance camera.");
}
void loop(){
  if(digitalRead(PIR)==LOW){screen("EN ATTENTE","Approchez-vous");delay(100);return;}
  screen("PRESENTEZ UN OBJET","Attente categorie");
  float d=distanceCm();
  if(d<=0||d>MAX_DISTANCE){delay(100);return;}
  if(digitalRead(BTN_ORGANIC)==LOW){while(digitalRead(BTN_ORGANIC)==LOW)delay(10);acceptCategory(1);}
  else if(digitalRead(BTN_RECYCLABLE)==LOW){while(digitalRead(BTN_RECYCLABLE)==LOW)delay(10);acceptCategory(2);}
  else if(digitalRead(BTN_RESIDUAL)==LOW){while(digitalRead(BTN_RESIDUAL)==LOW)delay(10);acceptCategory(3);}
  else {screen("SCAN EN ATTENTE","Aucune validation");delay(80);}
  closeAll();delay(500);
}
