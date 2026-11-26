/*
 * PLASTIC-BOT WOURILAB — ESP32-CAM AI-Thinker
 * Serveur de décision HTTP pour l'ESP32 principal.
 *
 * IMPORTANT : le classifieur IA n'est PAS encore entraîné/intégré.
 * Dans cet état, ce programme renvoie toujours accepted=false.
 * C'est volontaire : aucun déchet ne doit être accepté sur une
 * simple capture d'image ou sur une panne du modèle.
 *
 * Carte : AI Thinker ESP32-CAM
 * Bibliothèques : ESP32 Arduino core, ArduinoJson
 */
#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "VOTRE_WIFI";
const char* WIFI_PASSWORD = "VOTRE_MOT_DE_PASSE";
WebServer server(80);

// Brochage officiel du module ESP32-CAM AI Thinker.
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

struct Classification {
  bool accepted;
  float confidence;
  const char* reason;
};

// A REMPLACER par l'inférence du modèle entraîné.
// Par sécurité, retourne toujours false tant qu'un modèle testé
// n'a pas été intégré et validé sur des images réelles.
Classification classifyPlastic(const camera_fb_t* frame) {
  (void)frame;
  return {false, 0.0f, "MODEL_NOT_INSTALLED"};
}

void handleDecision() {
  camera_fb_t* frame = esp_camera_fb_get();
  if (!frame) {
    server.send(503, "application/json",
      "{\"accepted\":false,\"confidence\":0.0,\"reason\":\"CAMERA_CAPTURE_FAILED\"}");
    return;
  }

  Classification result = classifyPlastic(frame);
  esp_camera_fb_return(frame);

  // Le seuil est appliqué ici également : double verrouillage.
  const bool accepted = result.accepted && result.confidence >= 0.80f;
  JsonDocument response;
  response["accepted"] = accepted;
  response["confidence"] = result.confidence;
  response["reason"] = accepted ? "PLASTIC_CONFIRMED" : result.reason;
  String json;
  serializeJson(response, json);
  server.send(200, "application/json", json);
}

void handleRoot() {
  server.send(200, "text/plain",
    "PLASTIC-BOT WOURILAB ESP32-CAM: endpoint /decision");
}

void setup() {
  Serial.begin(115200);
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Erreur initialisation camera: 0x%x\n", err);
    while (true) delay(1000); // caméra indisponible = pas de décision
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connexion Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }
  Serial.printf("\nESP32-CAM IP: %s\n", WiFi.localIP().toString().c_str());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/decision", HTTP_GET, handleDecision);
  server.begin();
  Serial.println("Serveur HTTP actif sur /decision");
}

void loop() {
  server.handleClient();
}
