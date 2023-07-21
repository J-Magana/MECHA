// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#include <esp_task_wdt.h>

// Watchdog timer period in seconds
#define WDT_TIMEOUT 3

// Define Motor Connections
// Right Front Motor
#define MF_AI1 32
#define MF_AI2 23

// Left Front Motor
#define MF_BI1 33
#define MF_BI2 25

// Right Rear Motor
#define MR_AI1 12
#define MR_AI2 14

// Left Rear Motor
#define MR_BI1 13
#define MR_BI2 2

// Define a data structure for received data
typedef struct struct_message_rcv {
  int xAxis;
  int yAxis;
  bool pbSwitch;
} struct_message_rcv;

// Create a structured object for received data
struct_message_rcv rcvData;

// Define ESP-NOW timeout value
#define SIGNAL_TIMEOUT 500

// Last Receive time
unsigned long lastRecvTime = 0;

// Define Bytes to represent Mecanum Wheel Modes
// Individual bits define TB6612FNG motor driver module input states
// B7 = MF_AI1, B6 = MF_AI2, B5 = MF_BI1, B4 = MF_BI2, B3 = MR_AI1, B2 = MR_AI2, B1 = MR_BI1, B0 = MR_BI2
const byte MEC_STRAIGHT_FORWARD = B10101010;
const byte MEC_STRAIGHT_BACKWARD = B01010101;
const byte MEC_SIDEWAYS_RIGHT = B01101001;
const byte MEC_SIDEWAYS_LEFT = B10010110;
const byte MEC_DIAGONAL_45 = B00101000;
const byte MEC_DIAGONAL_135 = B10000010;
const byte MEC_DIAGONAL_225 = B00010100;
const byte MEC_DIAGONAL_315 = B01000001;
const byte MEC_ROTATE_CLOCKWISE = B01100110;
const byte MEC_ROTATE_COUNTERCLOCKWISE = B10011001;

// Variable for Motor Mode
byte motorModeValue = B00000000;

// Variable for Mecanum Mode
// 0 = Standard, 1 = Rotate, 2 = Pivot Right, 4 = Pivot Left, 5 = Pivot Front, 6 = Pivot Rear
int mecanumModeValue = 0;

// Variables for Joystick values
volatile int joyXaxis = 1880;
volatile int joyYaxis = 1900;

// Variable for Joystick pushbutton state
volatile bool joySwitchState = true;

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Disable WiFi sleep mode
  WiFi.setSleep(false);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    return;
  }

  // Register receive callback function
  esp_now_register_recv_cb(OnDataRecv);

  // Set all connections as outputs
  
  pinMode(MF_AI1, OUTPUT);
  pinMode(MF_AI2, OUTPUT);
 
  pinMode(MF_BI1, OUTPUT);
  pinMode(MF_BI2, OUTPUT);
  
  pinMode(MR_AI1, OUTPUT);
  pinMode(MR_AI2, OUTPUT);
  
  pinMode(MR_BI1, OUTPUT);
  pinMode(MR_BI2, OUTPUT);

  // Enable watchdog timer - true is "panic mode" to restart
  esp_task_wdt_init(WDT_TIMEOUT, true);
  
  // Add current thread to watchdog timer
  esp_task_wdt_add(NULL);

  // Stop all motors
  stopMotors();

  delay(1000);
}

void loop() {

  //Check timer to see if signal is lost
  unsigned long now = millis();

  if (now - lastRecvTime > SIGNAL_TIMEOUT)
  // Signal is lost
  {

    // Stop all motors
    stopMotors();

    // Set controller values to default
    joyXaxis = 1880;
    joyYaxis = 1900;
    joySwitchState = true;

  } 
  
  else {

    // Drive car using current mode
    driveMecanumMode();

  }

  // Reset watchdog timer
  esp_task_wdt_reset();

  // Short delay
  delay(50);
}