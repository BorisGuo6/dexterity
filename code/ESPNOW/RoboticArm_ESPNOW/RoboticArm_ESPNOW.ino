#include <esp_now.h>
#include <WiFi.h>

#define ESPNOW_WIFI_CHANNEL 5            // WiFi channel to be used by ESP-NOW. The available channels will depend on your region.
#define ESPNOW_WIFI_MODE    WIFI_STA     // WiFi mode to be used by ESP-NOW. Any mode can be used.
#define ESPNOW_WIFI_IF      WIFI_IF_STA  // WiFi interface to be used by ESP-NOW. Any interface can be used.

// Define the data structure to send
typedef struct position_packet {
  char str[32];
  int finger_position_1;
  int finger_angle_1;
  int wrist_position;
  int wrist_angle;
} position_packet;

typedef struct haptic_packet {
  char str[32];
  int finger_sensor_1;
  int finger_sensor_2;
  int finger_sensor_3;
} haptic_packet;

// Create an instance of the struct to be sent
position_packet inData;
haptic_packet outData;


// Peer MAC Address
uint8_t peer_mac[] = {0x3C, 0x84, 0x27, 0xE1, 0xB3, 0x8C};

// Function to handle the result of data send
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  if (len == sizeof(inData)) {
      memcpy(&inData, incomingData, sizeof(inData));
      // Proceed with processing
  } else {
      Serial.println("Received data length does not match expected size.");
  }
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Message: ");
  Serial.println(inData.str);
  Serial.print("finger_position_1: ");
  Serial.println(inData.finger_position_1);
  Serial.print("finger_angle_1: ");
  Serial.println(inData.finger_angle_1);
  Serial.print("wrist_position: ");
  Serial.println(inData.wrist_position);
  Serial.print("wrist_angle: ");
  Serial.println(inData.wrist_angle);
  Serial.println();
}


void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  // Set the WiFi to the mode and channel to be used by ESP-NOW
  Serial.println("Setting up WiFi...");
  WiFi.mode(ESPNOW_WIFI_MODE);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);

  // Wait for the WiFi to start
  while (!WiFi.STA.started()) {
    delay(100);
  }

  // Print the MAC address of this device
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  // DELAY: enter delay if trying to sync up MAC addresses
  // delay(10000);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback function
  esp_now_register_send_cb(OnDataSent);

  // Register the receive callback function
  esp_now_register_recv_cb(OnDataRecv);

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
}

void loop() {
  // Set values to send
  strcpy(outData.str, "Haptic Data");
  outData.finger_sensor_1 = random(1, 20);
  outData.finger_sensor_2 = random(1, 20);
  outData.finger_sensor_3 = random(1, 20);

  // Send struct message via ESP-NOW
  esp_err_t result = esp_now_send(peer_mac, (uint8_t *)&outData, sizeof(outData));

  if (result == ESP_OK) {
    // Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }

  delay(4000);  // Wait 4 seconds before sending again
}

