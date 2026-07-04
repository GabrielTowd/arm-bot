
#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>

// Pins des servos
const int servoPin1 = 0;
const int servoPin2 = 1;
const int servoPin3 = 2;

// Objets servo
Servo servo1;
Servo servo2;
Servo servo3;

// Structure reçue
typedef struct struct_message {
  int angle1;
  int angle2;
  int angle3;
} struct_message;

struct_message incomingData;

// Callback de réception
void onReceive(const esp_now_recv_info *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  int a1 = constrain(incomingData.angle1, 0, 180);
  int a2 = constrain(incomingData.angle2, 0, 180);
  int a3 = constrain(incomingData.angle3, 0, 180);

  servo1.write(a1);
  servo2.write(a2);
  servo3.write(a3);

  Serial.print("Servo1: ");
  Serial.print(a1);
  Serial.print(" | Servo2: ");
  Serial.print(a2);
  Serial.print(" | Servo3: ");
  Serial.println(a3);
}

void setup() {

  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  //Serial.println(WiFi.macAddress());

  // Attacher les servos
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(onReceive);
}

void loop() {
  // Rien ici → tout est géré par ESP-NOW
}