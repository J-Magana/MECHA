// Function to control motors with joystick values - Standard Mode
void motorControlMode0(int xjoystick, int yjoystick) {

  // Variables to hold mode byte
  byte motorCtlMode = B00000000;

  // Define variables
  int xaxis = xjoystick;
  int yaxis = yjoystick;

  // Determine Wheel Mode byte
  if ((xaxis == 0) && (yaxis > 0)) {
    //Straight Forward
    motorCtlMode = MEC_STRAIGHT_FORWARD;
  } 
  
  else if ((xaxis == 0) && (yaxis < 0)) {
    //Straight Backward
    motorCtlMode = MEC_STRAIGHT_BACKWARD;
  } 
  
  else if ((xaxis > 0) && (yaxis == 0)) {
    //Sideways Right
    motorCtlMode = MEC_SIDEWAYS_RIGHT;
  } 
  
  else if ((xaxis < 0) && (yaxis == 0)) {
    //Sideways Left
    motorCtlMode = MEC_SIDEWAYS_LEFT;
  } 
  
  else if ((xaxis > 0) && (yaxis > 0)) {
    //Diagonal 45 Degrees
    motorCtlMode = MEC_DIAGONAL_45;
  } 
  
  else if ((xaxis < 0) && (yaxis > 0)) {
    //Diagonal 135 Degrees
    motorCtlMode = MEC_DIAGONAL_135;
  } 
  
  else if ((xaxis < 0) && (yaxis < 0)) {
    //Diagonal 225 Degrees
    motorCtlMode = MEC_DIAGONAL_225;
  } 
  
  else if ((xaxis > 0) && (yaxis < 0)) {
    //Diagonal 315 Degrees
    motorCtlMode = MEC_DIAGONAL_315;
  }

  // Drive motors(moveMotors will correct any negative speed values)
  moveMotors(motorCtlMode);
}