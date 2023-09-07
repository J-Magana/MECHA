/*
  Modified from: https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <WiFi.h>
//#include <ezButton.h>
#include <esp_now.h>



#define VRX_PIN      36 // ESP32 pin GIOP36 (ADC0) connected to VRX pin
#define VRY_PIN      39 // ESP32 pin GIOP39 (ADC0) connected to VRY pin
#define SW_PIN       12 // ESP32 pin GIOP14 connected to SW  pin

//ezButton button(SW_PIN);

// REPLACE WITH YOUR RECEIVER MAC Address - GB3 as receiver
uint8_t broadcastAddress[] = {0x70, 0xB8, 0xF6, 0x99, 0x69, 0x70};


// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int valX;
  int valY;
  bool valZ;
} struct_message;

// Create a struct_message to transmit to pantilt
struct_message jsData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(SW_PIN, INPUT_PULLUP);
  //button.setDebounceTime(50); // set debounce time to 50 milliseconds
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  //button.loop(); // MUST call the loop() function first
  // Set values to send
  jsData.valX = analogRead(VRX_PIN);
  jsData.valY = analogRead(VRY_PIN);
  jsData.valZ = digitalRead(SW_PIN);

    
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &jsData, sizeof(jsData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(100);
}