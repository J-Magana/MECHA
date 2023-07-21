// Function to control motors with joystick values - Pivot Right Mode
void motorControlMode2(int yjoystick) {

  // Variables to hold mode byte and speed values
  byte motorCtlMode = B00000000;

  // Define variables and map Y joystick values to ranges from -255 to +255
  int yaxis = yjoystick;

  // Determine if forward or reverse
  if (yaxis > 0) {
    // Forward
    motorCtlMode = MEC_PIVOT_RIGHT_FORWARD;
  } 
  
  else if (yaxis < 0) {
    // Reverse
    motorCtlMode = MEC_PIVOT_RIGHT_BACKWARD;
  } 
  
  else {
    // Stopped
    motorCtlMode = B00000000;
  }

  // Drive motors(moveMotors will correct any negative speed values)
  moveMotors(motorCtlMode);
}

// Function to control motors with joystick values - Pivot Left Mode
void motorControlMode3(int yjoystick) {

  // Variables to hold mode byte
  byte motorCtlMode = B00000000;
  
  // Define variables
  int yaxis = yjoystick;

  // Determine if forward or reverse
  if (yaxis > 0) {
    // Forward
    motorCtlMode = MEC_PIVOT_LEFT_FORWARD;
  } 
  
  else if (yaxis < 0) {
    // Reverse
    motorCtlMode = MEC_PIVOT_LEFT_BACKWARD;
  } 
  
  else {
    // Stopped
    motorCtlMode = B00000000;
  }

  // Drive motors(moveMotors will correct any negative speed values)
  moveMotors(motorCtlMode);
}

// Function to control motors with joystick values - Pivot Front Mode
void motorControlMode4(int yjoystick) {

  // Variables to hold mode byte and speed values
  byte motorCtlMode = B00000000;

  // Define variables and map Y joystick values to ranges from -255 to +255
  int yaxis = yjoystick;

  // Determine if forward or reverse
  if (yaxis > 0) {
    // Forward
    motorCtlMode = MEC_PIVOT_SIDEWAYS_FRONT_RIGHT;
  } 
  
  else if (yaxis < 0) {
    // Reverse
    motorCtlMode = MEC_PIVOT_SIDEWAYS_FRONT_LEFT;
  } 
  
  else {
    // Stopped
    motorCtlMode = B00000000;
  }

  // Drive motors(moveMotors will correct any negative speed values)
  moveMotors(motorCtlMode);
}

// Function to control motors with joystick values - Pivot Rear Mode
void motorControlMode5(int yjoystick) {

  // Variables to hold mode byte and speed values
  byte motorCtlMode = B00000000;

  // Define variables
  int yaxis = yjoystick;

  // Determine if forward or reverse
  if (yaxis > 0) {
    // Forward
    motorCtlMode = MEC_PIVOT_SIDEWAYS_REAR_RIGHT;
  } 
  
  else if (yaxis < 0) {
    // Reverse
    motorCtlMode = MEC_PIVOT_SIDEWAYS_REAR_LEFT;
  } 
  
  else {
    // Stopped
    motorCtlMode = B00000000;
  }

  // Drive motors & set LED colors (moveMotors will correct any negative speed values)
  moveMotors(motorCtlMode);
}

