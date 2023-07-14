// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>

Servo gripper;  // create servo object to control a servo
Servo tilter;  // create servo object to control a servo
Servo lifter;


// Replace with your network credentials
const char* ssid = "KennethAP";
const char* password = "12345678";

//int posVal = 0;    // variable to store the servo position
int gripPin = 21; // Servo motor pin
int tiltPin = 15; // Servo motor pin
int liftPin = 5 ;

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

void setup() { 
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  //setup servo
  gripper.setPeriodHertz(50);           // standard 50 hz servo
  gripper.attach(gripPin, 500, 2400);  // attaches the servo to the servo object
  gripper.write(gripperValue.toInt());

  //setup servo
  tilter.setPeriodHertz(50);           // standard 50 hz servo
  tilter.attach(tiltPin, 700, 1300);  // attaches the servo to the servo object
  tilter.write(tilterValue.toInt());

  //setup servo
  lifter.setPeriodHertz(50);
  lifter.attach(liftPin, 700, 1700);  // attaches the servo to the servo object
  lifter.write(lifterValue.toInt());

  // Create SoftAP
WiFi.softAP(ssid, password);
Serial.print("Connect to access point: ");
Serial.println(ssid); //ssid will show up on serial monitor
Serial.println(String("Soft-AP IP address = ") + WiFi.softAPIP().toString());
//look for the ssid on PC's network connections and connect
//use password set above and then open a browser with the IP address, default value is
//192.168.4.1

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
  
}