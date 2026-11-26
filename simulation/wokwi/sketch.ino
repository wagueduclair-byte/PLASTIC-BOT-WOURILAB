/*
 * Simulation logique Wokwi : le bouton simule successivement les 3 classes.
 * Ce programme ne réalise PAS de reconnaissance d'image/IA.
 */
#include <Arduino.h>
#include <ESP32Servo.h>
constexpr int SERVO_PIN=18, BUTTON_PIN=27;
constexpr int NEUTRAL=90, ORGANIC=35, RECYCLABLE=90, RESIDUAL=145;
Servo chute; uint8_t category=0; bool previous=HIGH;
void setup(){
  Serial.begin(115200); pinMode(BUTTON_PIN,INPUT_PULLUP);
  chute.setPeriodHertz(50); chute.attach(SERVO_PIN); chute.write(NEUTRAL);
  Serial.println("DEMO SIMULEE : entree unique, trois destinations.");
}
void loop(){
  bool current=digitalRead(BUTTON_PIN);
  if(previous==HIGH && current==LOW){
    category=(category+1)%3;
    if(category==0){Serial.println("Simulation ORGANIC");chute.write(ORGANIC);}
    else if(category==1){Serial.println("Simulation RECYCLABLE");chute.write(RECYCLABLE);}
    else {Serial.println("Simulation RESIDUAL");chute.write(RESIDUAL);}
    delay(700); chute.write(NEUTRAL);
  }
  previous=current; delay(20);
}
