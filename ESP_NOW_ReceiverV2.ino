#include "esp_now.h"
#include "WiFi.h"

int ledPin=2;

typedef struct struct_message{
  int throttle;
  int z;
  int y;
  int x;
} struct_message;

struct_message joystick;

void OnDataRecv(const uint8_t *mac,const uint8_t *incomingData, int len){
  memcpy(&joystick, incomingData, sizeof(joystick));
  Serial.print("Data received: ");
  Serial.println(joystick.throttle);
  Serial.println(joystick.z);
  Serial.println(joystick.y);
  Serial.println(joystick.x);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if(esp_now_init()!= ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  pinMode(ledPin,OUTPUT);
}

void loop() {
    digitalWrite(ledPin,joystick.y);
}