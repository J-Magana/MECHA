// Function to control motors with joystick values - Standard Mode
void motorControlMode() {

  // Variables to hold mode byte
  byte motorCtlMode = B00000000;

  // Define variables
  int xaxis = jsData.valX;
  int yaxis = jsData.valY;

  // Determine Wheel Mode byte
  if (((xaxis >= 1000) && (xaxis <= 3000)) && (yaxis > 3000)) {
    //Straight Forward
    motorCtlMode = MEC_STRAIGHT_FORWARD;
  } 
  
  else if (((xaxis >= 1000) && (xaxis <= 3000)) && (yaxis < 1000)) {
    //Straight Backward
    motorCtlMode = MEC_STRAIGHT_BACKWARD;
  } 
  
  else if ((xaxis < 1000) && ((yaxis >= 1000) && (yaxis <= 3000))) {
    //Sideways Right
    motorCtlMode = MEC_SIDEWAYS_RIGHT;
  } 
  
  else if ((xaxis > 3000) && ((yaxis >= 1000) && (yaxis <= 3000))) {
    //Sideways Left
    motorCtlMode = MEC_SIDEWAYS_LEFT;
  } 
  
  else if ((xaxis < 1000) && (yaxis > 3000)) {
    //Diagonal 45 Degrees
    motorCtlMode = MEC_DIAGONAL_45;
  } 
  
  else if ((xaxis > 3000) && (yaxis > 3000)) {
    //Diagonal 135 Degrees
    motorCtlMode = MEC_DIAGONAL_135;
  } 
  
  else if ((xaxis > 3000) && (yaxis < 1000)) {
    //Diagonal 225 Degrees
    motorCtlMode = MEC_DIAGONAL_225;
  } 
  
  else if ((xaxis < 1000) && (yaxis < 1000)) {
    //Diagonal 315 Degrees
    motorCtlMode = MEC_DIAGONAL_315;
  }

  else {
    // Stopped
    motorCtlMode = B00000000;
  }

  // Drive motors(moveMotors will correct any negative speed values)
  moveMotors(motorCtlMode);
}