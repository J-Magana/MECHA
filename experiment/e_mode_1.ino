// // Function to control motors with joystick values - Rotate Mode
// void motorControlMode1(int jsData.valY) {

//   // Variables to hold mode byte and speed values
//   byte motorCtlMode = B00000000;

//   // Define variables
//   int yaxis = jsData.valY;

//   // Determine if forward or reverse
//   if (yaxis > 2000) {
//     // Forward
//     motorCtlMode = MEC_ROTATE_CLOCKWISE;
//   } 
  
//   else if (yaxis < 1800) {
//     // Reverse
//     motorCtlMode = MEC_ROTATE_COUNTERCLOCKWISE;
//   } 
  
//   else {
//     // Stopped
//     motorCtlMode = B00000000;
//   }

//   // Drive motors & set LED colors (moveMotors will correct any negative speed values)
//   moveMotors(motorCtlMode);
// }