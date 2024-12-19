#include "esp_now.h"
#include "WiFi.h"

const int pinVRYL = 32;
const int pinVRXL = 33;
const int pinVRYR = 34;
const int pinVRXR = 35;

uint8_t broadcastAddress[]={0xA8,0x42,0xE3,0xCB,0x78,0xEC};

typedef struct struct_message{
  int throttle;
  int z;
  int y;
  int x;

} struct_message;

struct_message joystick;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr,esp_now_send_status_t status){
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status==ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if(esp_now_init()!= ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel=0;
  peerInfo.encrypt=false;

  if(esp_now_add_peer(&peerInfo)!=ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  pinMode(pinVRYL,INPUT);
  pinMode(pinVRXL,INPUT);
  pinMode(pinVRYR,INPUT);
  pinMode(pinVRXR,INPUT);
}

void loop() {
  joystick.throttle = map(analogRead(pinVRXL),0,4095,1000,2000);
  delay(10);
  joystick.z        = map(analogRead(pinVRYL),0,4095,1000,2000);
  delay(10);
  joystick.y        = map(analogRead(pinVRXR),0,4095,1000,2000);
  delay(10);
  joystick.x        = map(analogRead(pinVRYR),0,4095,1000,2000);

  esp_err_t result=esp_now_send(broadcastAddress, (uint8_t *) &joystick, sizeof(joystick));

  if(result==ESP_OK)
    Serial.println("Sending confirmed");
  else
    Serial.println("Sending error");
  delay(10);

}
