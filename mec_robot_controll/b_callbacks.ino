// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

  if (status == ESP_NOW_SEND_SUCCESS) {
    connectStatus = "Car found";
    connectError = LOW;
  } 
  
  else {
    connectStatus = "Car not found";
    connectError = HIGH;
  }
}

