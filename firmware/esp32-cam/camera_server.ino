/*
 * PLASTIC-BOT WOURILAB — ESP32-CAM AI Thinker, scanner 3 catégories.
 * API GET /decision : {"category":"organic","confidence":0.93}
 * IMPORTANT : modèle IA non encore entraîné/intégré.
 * En attendant, unknown/0.0 est renvoyé pour garder les entrées fermées.
 */
#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
const char* WIFI_SSID="VOTRE_WIFI";
const char* WIFI_PASSWORD="VOTRE_MOT_DE_PASSE";
WebServer server(80);

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

struct Classification { const char* category; float confidence; };
// Remplacer par l'inférence du modèle entraîné. Fail-safe par défaut.
Classification classifyWaste(const camera_fb_t* frame) {
  (void)frame;
  return {"unknown",0.0f};
}
void handleDecision() {
  camera_fb_t* frame=esp_camera_fb_get();
  if(!frame) {
    server.send(503,"application/json","{\"category\":\"unknown\",\"confidence\":0.0}");
    return;
  }
  Classification r=classifyWaste(frame);
  esp_camera_fb_return(frame);
  bool valid=String(r.category)=="organic" || String(r.category)=="recyclable" || String(r.category)=="residual";
  if(!valid || r.confidence<0.80f) r={"unknown",0.0f};
  JsonDocument doc; doc["category"]=r.category; doc["confidence"]=r.confidence;
  String body; serializeJson(doc,body);
  server.send(200,"application/json",body);
}
void setup() {
  Serial.begin(115200);
  camera_config_t c;
  c.ledc_channel=LEDC_CHANNEL_0; c.ledc_timer=LEDC_TIMER_0;
  c.pin_d0=Y2_GPIO_NUM; c.pin_d1=Y3_GPIO_NUM; c.pin_d2=Y4_GPIO_NUM; c.pin_d3=Y5_GPIO_NUM;
  c.pin_d4=Y6_GPIO_NUM; c.pin_d5=Y7_GPIO_NUM; c.pin_d6=Y8_GPIO_NUM; c.pin_d7=Y9_GPIO_NUM;
  c.pin_xclk=XCLK_GPIO_NUM; c.pin_pclk=PCLK_GPIO_NUM; c.pin_vsync=VSYNC_GPIO_NUM; c.pin_href=HREF_GPIO_NUM;
  c.pin_sccb_sda=SIOD_GPIO_NUM; c.pin_sccb_scl=SIOC_GPIO_NUM;
  c.pin_pwdn=PWDN_GPIO_NUM; c.pin_reset=RESET_GPIO_NUM;
  c.xclk_freq_hz=20000000; c.pixel_format=PIXFORMAT_JPEG;
  c.frame_size=FRAMESIZE_QVGA; c.jpeg_quality=12; c.fb_count=1;
  c.grab_mode=CAMERA_GRAB_WHEN_EMPTY; c.fb_location=CAMERA_FB_IN_PSRAM;
  if(esp_camera_init(&c)!=ESP_OK) { Serial.println("Camera indisponible"); while(true) delay(1000); }
  WiFi.mode(WIFI_STA); WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while(WiFi.status()!=WL_CONNECTED) delay(400);
  Serial.printf("ESP32-CAM IP: %s\n",WiFi.localIP().toString().c_str());
  server.on("/",HTTP_GET,[]{server.send(200,"text/plain","PLASTIC-BOT WOURILAB - scanner trois categories");});
  server.on("/decision",HTTP_GET,handleDecision);
  server.begin();
}
void loop(){ server.handleClient(); }
