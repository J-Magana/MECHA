void toggleMecanumMode() {
  // Toggle Mecanum Mode value from 0 to 5
  int currentMecMode = mecanumModeValue;
  
  if (currentMecMode == 1) {
    mecanumModeValue = 0;
  }
  
  else {
    mecanumModeValue = currentMecMode + 1;
  }
}

void driveMecanumMode() {

  // Pass control to proper Mecanum Mode function
  switch (mecanumModeValue) {

    case 0:
      // Standard driving mode, controlled by joystick
      // Pass joystick values to motorControlMode0 function
      motorControlMode0(joyXaxis, joyYaxis);

      break;

    case 1:
      // Rotate mode, controlled by joystick Y Axis only
      // Pass joystick values to motorControlMode0 function
      motorControlMode1(joyYaxis);

      break;
  }
}

