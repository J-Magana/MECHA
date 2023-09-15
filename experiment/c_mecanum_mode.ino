// void toggleMecanumMode() {
//   // Toggle Mecanum Mode value from 0 to 5
//   int currentMecMode = mecanumModeValue;
  
//   if (currentMecMode == 1) {
//     mecanumModeValue = 0;
//   }
  
//   else {
//     mecanumModeValue = currentMecMode + 1;
//   }
// }

// void driveMecanumMode() {

//   // Pass control to proper Mecanum Mode function
//   switch (mecanumModeValue) {

//     case 0:
//       // Standard driving mode, controlled by joystick
//       // Pass joystick values to motorControlMode0 function
//       motorControlMode0(jsData.valX, jsData.valY);
//       Serial.println("case 1");

//       break;

//     case 1:
//       // Rotate mode, controlled by joystick Y Axis only
//       // Pass joystick values to motorControlMode0 function
//       motorControlMode1(jsData.valY);
//       Serial.println("case 2");

//       break;
//   }
// }void toggleMecanumMode() {
//   // Toggle Mecanum Mode value from 0 to 5
//   int currentMecMode = mecanumModeValue;
  
//   if (currentMecMode == 1) {
//     mecanumModeValue = 0;
//   }
  
//   else {
//     mecanumModeValue = currentMecMode + 1;
//   }
// }

// void driveMecanumMode() {

//   // Pass control to proper Mecanum Mode function
//   switch (mecanumModeValue) {

//     case 0:
//       // Standard driving mode, controlled by joystick
//       // Pass joystick values to motorControlMode0 function
//       motorControlMode0(jsData.valX, jsData.valY);
//       Serial.println("case 1");

//       break;

//     case 1:
//       // Rotate mode, controlled by joystick Y Axis only
//       // Pass joystick values to motorControlMode0 function
//       motorControlMode1(jsData.valY);
//       Serial.println("case 2");

//       break;
//   }
// }