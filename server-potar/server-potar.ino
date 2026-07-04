#include <WiFi.h>
#include <esp_now.h>

// Adresse MAC du récepteur (à modifier)
uint8_t receiverMAC[] = {0x24, 0x6F, 0x28, 0xAB, 0xCD, 0xEF};

// Pins des potentiomètres
const int potPin1 = 0;
const int potPin2 = 1;
const int potPin3 = 2;

// Structure envoyée
typedef struct struct_message {
  int angle1;
  int angle2;
  int angle3;
} struct_message;

struct_message dataToSend;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);
}

void loop() {

  // Lire les potentiomètres
  int val1 = analogRead(potPin1);
  int val2 = analogRead(potPin2);
  int val3 = analogRead(potPin3);

  // Convertir en angle
  dataToSend.angle1 = map(val1, 0, 4095, 0, 180);
  dataToSend.angle2 = map(val2, 0, 4095, 0, 180);
  dataToSend.angle3 = map(val3, 0, 4095, 0, 180);

  // Envoyer les données
  esp_now_send(receiverMAC, (uint8_t *) &dataToSend, sizeof(dataToSend));

  // Debug
  Serial.print("A1: "); Serial.print(dataToSend.angle1);
  Serial.print(" | A2: "); Serial.print(dataToSend.angle2);
  Serial.print(" | A3: "); Serial.println(dataToSend.angle3);

  delay(50);
}