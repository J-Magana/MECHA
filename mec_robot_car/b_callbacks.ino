// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  
}

// Callback function executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {

  if (len == 0) {
    // No data received, Stop all motors
    stopMotors();

    // Set controller values to default
    joyXaxis = 127;
    joyYaxis = 127;
    joySwitchState = true;
    return;
  }

  memcpy(&rcvData, incomingData, sizeof(rcvData));

  // Pass received values to local variables
  joyXaxis = rcvData.xAxis;
  joyYaxis = rcvData.yAxis;
  joySwitchState = rcvData.pbSwitch;

  // Check to see if we are changing Mecanum mode
  if (joySwitchState == true) {
    // Switch was pressed, toggle mecanum mode
    toggleMecanumMode();
  }

  // Update last received time counter
  lastRecvTime = millis();
}

