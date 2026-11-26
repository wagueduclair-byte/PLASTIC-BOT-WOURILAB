// Simulation Wokwi — la validation caméra est simulée par une variable.
// Modifier CAMERA_ACCEPTS pour tester acceptation/refus.
#include <ESP32Servo.h>
const int TRIG=5, ECHO=18, SERVO_PIN=19, LED_OK=2, LED_NO=4, BUZZER=23;
const bool CAMERA_ACCEPTS = false; // false = sécurité : refus par défaut
Servo lid;
float distanceCm(){
  digitalWrite(TRIG,LOW); delayMicroseconds(3);
  digitalWrite(TRIG,HIGH); delayMicroseconds(10); digitalWrite(TRIG,LOW);
  unsigned long t=pulseIn(ECHO,HIGH,30000);
  return t ? t*0.0343/2.0 : -1;
}
void setup(){
  Serial.begin(115200);
  pinMode(TRIG,OUTPUT); pinMode(ECHO,INPUT);
  pinMode(LED_OK,OUTPUT); pinMode(LED_NO,OUTPUT); pinMode(BUZZER,OUTPUT);
  lid.attach(SERVO_PIN); lid.write(0);
}
void loop(){
  float d=distanceCm();
  if(d>0 && d<=18){
    Serial.println("Objet presente : simulation validation camera");
    if(CAMERA_ACCEPTS){
      digitalWrite(LED_OK,HIGH); lid.write(85); tone(BUZZER,1400,120);
      delay(2500); lid.write(0); digitalWrite(LED_OK,LOW);
    } else {
      digitalWrite(LED_NO,HIGH); tone(BUZZER,700,250);
      delay(500); digitalWrite(LED_NO,LOW);
    }
    delay(4000);
  }
  delay(100);
}