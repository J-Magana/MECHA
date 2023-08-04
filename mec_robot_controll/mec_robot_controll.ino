// Include Libraries for ESP-NOW Communications
#include <esp_now.h>
#include <WiFi.h>

// Define Joystick Connections (some joystick models reverse X & Y)
#define X_AXIS_PIN 33
#define Y_AXIS_PIN 32
#define SWITCH_PIN 27

// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = { 0x70, 0xB8, 0xF6, 0x99, 0x69, 0x70 };

// Create a structured object for sent data
typedef struct struct_message_xmit {
  int xAxis;
  int yAxis;
  bool pbSwitch;
} struct_message_xmit;

// Create a structured object for sent data
struct_message_xmit xmitData;

// ESP-NOW Peer info
esp_now_peer_info_t peerInfo;

// Variable for Motor Mode
volatile byte motorModeValue = B00000000;

// Variable for Mecanum Mode
volatile int mecanumModeValue = 0;

// Variables for Joystick values
int joyXaxis = 1880;
int joyYaxis = 1900;

// Variable for Joystick pushbutton state
bool joySwitchState = HIGH;

// Variable for connection error  - HIGH is error state
volatile bool connectError = LOW;

// Variable for connection status string
String connectStatus = "NO INFO";

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);

  // Set joystick pin as input with Pullup
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Disable WiFi Sleep mode
  WiFi.setSleep(false);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    connectStatus = "ESP-NOW Error";
    connectError = HIGH;
    return;
  } 
  
  else {
    connectStatus = "ESP-NOW OK";
    connectError = LOW;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    connectStatus = "No peer added";
    connectError = HIGH;
    Serial.println("Failed to add peer");
    return;
  } 
  
  else {
    connectStatus = "ESP-NOW Ready";
    connectError = LOW;
    Serial.println("add peer");
  }

  // Enter the Loop with connectError set HIGH to avoid intial display flicker
  connectError = HIGH;
}

void loop() {

  // Check connection status
  if (connectError == LOW) {

    // Get joystick values and convert them
    joyXaxis = analogRead(X_AXIS_PIN);
    joyYaxis = analogRead(Y_AXIS_PIN);

  } 
  
  else {
    // Send "zero" values as joystick data
    joyXaxis = 1880;
    joyYaxis = 1900;
  }

  // Check and set switch status
  if (digitalRead(SWITCH_PIN) == LOW) {
    // Switch was pressed
    joySwitchState = true;
  } 
  
  else {
    joySwitchState = false;
  }

  // Format structured data
  xmitData.xAxis = joyXaxis;
  xmitData.yAxis = joyYaxis;
  xmitData.pbSwitch = joySwitchState;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&xmitData, sizeof(xmitData));

  // If switch was pressed delay longer to debounce
  if (joySwitchState == true) {
    delay(200);
  } else {
    delay(50);
  }
}

