#include "RoboticArm_ESPNOW.h"

// Start RoboticArm_ESPNOW.c:

uint8_t peer_mac[6];

position_packet arm_inData;
haptic_packet arm_outData;

// remove when not monitoring success rate:
int arm_messages_send_attempt = 0;
int arm_messages_send_success = 0;
int arm_messages_rcv = 0;

// remove when not monitoring success rate:
uint8_t pos5_in;
int sizeIn;
// end remove

// Function to handle the result of data send
void ArmOnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback function that will be executed when data is received
void ArmOnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {  // Changed signature to match expected esp_now_recv_cb_t type
  if (len == sizeof(arm_inData)) {
      memcpy(&arm_inData, incomingData, sizeof(arm_inData));
      // Proceed with processing
  } else {
      Serial.println("Received data length does not match expected size.");
  }
  arm_messages_send_success = arm_inData.messages_rec;
  arm_messages_rcv++;
  pos5_in = arm_inData.finger_pos[5];
  sizeIn = len;
}

void arm_ESPNOWsetup(uint8_t mac_in[], int baud_rate){
  Serial.begin(baud_rate);

  arm_messages_send_attempt = 0;
  arm_messages_send_success = 0;
  arm_messages_rcv = 0;
  
  for(int i=0; i<6; i++){
    peer_mac[i] = mac_in[i];
  }

  // Set the WiFi to the mode and channel to be used by ESP-NOW
  Serial.println("Setting up WiFi...");
  WiFi.mode(ESPNOW_WIFI_MODE);
  
  // WiFi.setChannel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);  // Replaced this line
  esp_wifi_set_channel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);  // Correct method to set channel on ESP32

  // Wait for the WiFi to start (removed unnecessary check for WiFi.STA)
  // Print the MAC address of this device
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback function
  esp_now_register_send_cb(ArmOnDataSent);

  // Register the receive callback function
  esp_now_register_recv_cb(ArmOnDataRecv);

  // Add the peer (receiver) device
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));  // Clear the peer info
  memcpy(peerInfo.peer_addr, peer_mac, 6); // Set peer MAC address
  peerInfo.channel = ESPNOW_WIFI_CHANNEL;  // Same WiFi channel as sender
  peerInfo.encrypt = false;  // No encryption
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // DELAY: enter delay if trying to sync up MAC addresses
  delay(SYNC_DELAY*1000);
}

void arm_ESPNOWsetup(uint8_t mac_in[]){
  arm_messages_send_attempt = 0;
  arm_messages_send_success = 0;
  arm_messages_rcv = 0;
  
  for(int i=0; i<6; i++){
    peer_mac[i] = mac_in[i];
  }

  // Set the WiFi to the mode and channel to be used by ESP-NOW
  Serial.println("Setting up WiFi...");
  WiFi.mode(ESPNOW_WIFI_MODE);
  
  // WiFi.setChannel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);  // Replaced this line
  esp_wifi_set_channel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);  // Correct method to set channel on ESP32

  // Wait for the WiFi to start (removed unnecessary check for WiFi.STA)
  // Print the MAC address of this device
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback function
  esp_now_register_send_cb(ArmOnDataSent);

  // Register the receive callback function
  esp_now_register_recv_cb(ArmOnDataRecv);

  // Add the peer (receiver) device
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));  // Clear the peer info
  memcpy(peerInfo.peer_addr, peer_mac, 6); // Set peer MAC address
  peerInfo.channel = ESPNOW_WIFI_CHANNEL;  // Same WiFi channel as sender
  peerInfo.encrypt = false;  // No encryption
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // DELAY: enter delay if trying to sync up MAC addresses
  delay(SYNC_DELAY*1000);
}

void arm_sendData(uint8_t fi, uint8_t fm, uint8_t fr, uint8_t fp, uint8_t ft){
  // Set values to send
  arm_outData.force_index = fi;
  arm_outData.force_middle = fm;
  arm_outData.force_ring = fr;
  arm_outData.force_pinky = fp;
  arm_outData.force_thumb = ft;
  arm_outData.messages_rec = arm_messages_rcv;

  // Send struct message via ESP-NOW
  esp_err_t result = esp_now_send(peer_mac, (uint8_t *)&arm_outData, sizeof(arm_outData));

  arm_messages_send_attempt += 1;

  if (result == ESP_OK) {
    // Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
}

void arm_monitorSuccess(){
  Serial.println();
  Serial.print("Messages Sent: ");
  Serial.println(arm_messages_send_attempt);
  Serial.print("Messages Delivered: ");
  Serial.println(arm_messages_send_success);
  Serial.print("Success rate: ");
  Serial.print(arm_messages_send_success*100/arm_messages_send_attempt);
  Serial.println(" %");
  Serial.print("position 5: ");
  Serial.println(pos5_in);
  Serial.print("size in: ");
  Serial.println(sizeIn);
  Serial.println();
}

// End RoboticArm_ESPNOW.c
