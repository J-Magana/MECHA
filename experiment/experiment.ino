// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <esp_now.h>


Servo gripper;  // create servo object to control a servo
Servo tilter;  // create servo object to control a servo
Servo lifter;  // create servo object to control a servo



// Structure to receive data, matches sender structure
typedef struct struct_message {
  int valX;
  int valY;
  bool valZ;
} struct_message;

// Create a struct_message called jsData
struct_message jsData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&jsData, incomingData, sizeof(jsData));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  Serial.print("RecvX: ");
  Serial.print(jsData.valX);
  Serial.print(",  RecvY: ");
  Serial.print(jsData.valY);
  Serial.print(",  RecvZ: ");
  Serial.println(jsData.valZ);

  //Pass recieved values to local variables and print
  // int valueX = jsData.valX;
  // int valueY = jsData.valY;
  // int valueZ = jsData.valZ;
}

// AP connection - Replace with your network credentials
const char* ssid = "MechArmAP";
const char* password = "12345678";

//Motor Connections
// Right Front Motor
#define MF_AI1 19
#define MF_AI2 21

// Left Front Motor
#define MF_BI1 18
#define MF_BI2 4

// Right Rear Motor
#define MR_AI1 25
#define MR_AI2 12

// Left Rear Motor
#define MR_BI1 33
#define MR_BI2 32

//motor direction control
const byte MEC_STRAIGHT_FORWARD = B10101010;
const byte MEC_STRAIGHT_BACKWARD = B01010101;
const byte MEC_SIDEWAYS_RIGHT = B01101001;
const byte MEC_SIDEWAYS_LEFT = B10010110;
const byte MEC_DIAGONAL_45 = B00101000;
const byte MEC_DIAGONAL_135 = B10000010;
const byte MEC_DIAGONAL_225 = B00010100;
const byte MEC_DIAGONAL_315 = B01000001;
const byte MEC_PIVOT_RIGHT_FORWARD = B00100010;
const byte MEC_PIVOT_RIGHT_BACKWARD = B00010001;
const byte MEC_PIVOT_LEFT_FORWARD = B10001000;
const byte MEC_PIVOT_LEFT_BACKWARD = B01000100;
const byte MEC_ROTATE_CLOCKWISE = B01100110;
const byte MEC_ROTATE_COUNTERCLOCKWISE = B10011001;
const byte MEC_PIVOT_SIDEWAYS_FRONT_RIGHT = B01100000;
const byte MEC_PIVOT_SIDEWAYS_FRONT_LEFT = B10010000;
const byte MEC_PIVOT_SIDEWAYS_REAR_RIGHT = B00001001;
const byte MEC_PIVOT_SIDEWAYS_REAR_LEFT = B00000110;

// variables to store the servo position
int gripPin = 5; // Servo motor pin
int tiltPin = 2; // Servo motor pin
int liftPin = 27;

String gripperValue = "0";
String tilterValue = "0";
String lifterValue = "0";

const char* PARAM_INPUTG = "gvalue";
const char* PARAM_INPUTT = "tvalue";
const char* PARAM_INPUTL = "lvalue";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Mechanical Arm Rover</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
  </style>
</head>
<body>
  <h2>Mech Arm Rover</h2>
  <p>Gripper: <span id="textGrip">%GRIPPERVALUE%</span></p>
  <p><input type="range" onchange="updateGripper(this)" id="gripRange" min="0" max="180" gvalue="%GRIPPERVALUE%" step="1" class="slider"></p>
  <p>Tilter: <span id="textTilt">%TILTERVALUE%</span></p>
  <p><input type="range" onchange="updateTilter(this)" id="tiltRange" min="0" max="180" tvalue="%TILTERVALUE%" step="1" class="slider"></p>
  <p>Lifter: <span id="textLift">%LIFTERVALUE%</span></p>
  <p><input type="range" onchange="updateLifter(this)" id="liftRange" min="0" max="180" lvalue="%LIFTERVALUE%" step="1" class="slider"></p>
<script>
function updateGripper(element) {
  var gripperValue = document.getElementById("gripRange").value;
  document.getElementById("textGrip").innerHTML = gripperValue;
  console.log(gripperValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?gvalue="+gripperValue, true);
  xhr.send();
}
function updateTilter(element) {
  var tilterValue = document.getElementById("tiltRange").value;
  document.getElementById("textTilt").innerHTML = tilterValue;
  console.log(tilterValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?tvalue="+tilterValue, true);
  xhr.send();
}
function updateLifter(element) {
  var lifterValue = document.getElementById("liftRange").value;
  document.getElementById("textLift").innerHTML = lifterValue;
  console.log(lifterValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?lvalue="+lifterValue, true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
    if (var == "GRIPPERVALUE"){
    return gripperValue;
    }
    if (var == "TILTERVALUE"){
    return tilterValue;
    }
    if (var == "LIFTERVALUE"){
    return lifterValue;
    }
  return String();
  }

//motor functions - Directions specified in direction byte
void moveMotors(byte dircontrol) {
  // Right Front Motor
  digitalWrite(MF_AI1, bitRead(dircontrol, 7));
  digitalWrite(MF_AI2, bitRead(dircontrol, 6));
  
  // Left Front Motor
  digitalWrite(MF_BI1, bitRead(dircontrol, 5));
  digitalWrite(MF_BI2, bitRead(dircontrol, 4));
  
  // Right Rear Motor
  digitalWrite(MR_AI1, bitRead(dircontrol, 3));
  digitalWrite(MR_AI2, bitRead(dircontrol, 2));
 
  // Left Rear Motor
  digitalWrite(MR_BI1, bitRead(dircontrol, 1));
  digitalWrite(MR_BI2, bitRead(dircontrol, 0));
}
void stopMotors() {
  digitalWrite(MF_AI1, 0);
  digitalWrite(MF_AI2, 0);
  digitalWrite(MF_BI1, 0);
  digitalWrite(MF_BI2, 0);
  digitalWrite(MR_AI1, 0);
  digitalWrite(MR_AI2, 0);
  digitalWrite(MR_BI1, 0);
  digitalWrite(MR_BI2, 0);
}

void setup() { 
  // Serial monitor 
  Serial.begin(115200);
  //setup servos
  gripper.setPeriodHertz(50);           // standard 50 hz servo
  gripper.attach(gripPin, 500, 2400);  // attaches the servo to the servo object
  gripper.write(gripperValue.toInt());
 
  tilter.setPeriodHertz(50);           // standard 50 hz servo
  tilter.attach(tiltPin, 700, 1300);  // attaches the servo to the servo object
  tilter.write(tilterValue.toInt());

   lifter.setPeriodHertz(50);
  lifter.attach(liftPin, 700, 1700);  // attaches the servo to the servo object
  lifter.write(lifterValue.toInt());

  // Set motor connections as outputs
  pinMode(MF_AI1, OUTPUT);
  pinMode(MF_AI2, OUTPUT);
  pinMode(MF_BI1, OUTPUT);
  pinMode(MF_BI2, OUTPUT);
  pinMode(MR_AI1, OUTPUT);
  pinMode(MR_AI2, OUTPUT);
  pinMode(MR_BI1, OUTPUT);
  pinMode(MR_BI2, OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_AP_STA);

  // Create SoftAP  
WiFi.softAP(ssid, password);
Serial.print("Connect to access point: ");
Serial.println(ssid); //ssid will show up on serial monitor
Serial.println(String("Soft-AP IP address = ") + WiFi.softAPIP().toString());
//look for the ssid on PC's network connections and connect
//use password set above and then open a browser with the IP address, default value is
//192.168.4.1

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUTG)) {
      inputMessage = request->getParam(PARAM_INPUTG)->value();
      gripperValue = inputMessage;
      gripper.write(gripperValue.toInt());
    }
     else if (request->hasParam(PARAM_INPUTT)) {
      inputMessage = request->getParam(PARAM_INPUTT)->value();
      tilterValue = inputMessage;
      tilter.write(tilterValue.toInt());
    }
      else if (request->hasParam(PARAM_INPUTL)) {
      inputMessage = request->getParam(PARAM_INPUTL)->value();
      lifterValue = inputMessage;
      lifter.write(lifterValue.toInt());
      }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}
  
void loop() {

  motorControlMode();
  
}