#include <esp_now.h>
#include <WiFi.h>
#include <stdint.h>

#define ESPNOW_WIFI_CHANNEL 5            // WiFi channel to be used by ESP-NOW. The available channels will depend on your region.
#define ESPNOW_WIFI_MODE    WIFI_STA     // WiFi mode to be used by ESP-NOW. Any mode can be used.
#define ESPNOW_WIFI_IF      WIFI_IF_STA  // WiFi interface to be used by ESP-NOW. Any interface can be used.
#define DATA_RATE           100          // In Hz

// Define the data structure to send
typedef struct position_packet {
  char str[32];
  int messages_rec;
  uint8_t pos1;
  uint8_t pos2;
  uint8_t pos3;
  uint8_t pos4;
  uint8_t pos5;
  uint8_t pos6;
  uint8_t pos7;
  uint8_t pos8;
  uint8_t pos9;
  uint8_t pos10;
  uint8_t pos11;
  uint8_t pos12;
  uint8_t pos13;
  uint8_t pos14;
  uint8_t pos15;
  uint8_t pos16;
} position_packet;

typedef struct haptic_packet {
  char str[32];
  int messages_rec;
  int finger_sensor_1;
  int finger_sensor_2;
  int finger_sensor_3;
} haptic_packet;

// Create an instance of the struct to be sent
position_packet outData;
haptic_packet inData;

int messages_send_attempt;
int messages_send_success;
int messages_recv;
int i;

// PEER MAC: 3C:84:27:14:7B:B0
uint8_t peer_mac[] = {0x3C, 0x84, 0x27, 0x14, 0x7B, 0xB0};

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
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Message: ");
  // Serial.println(inData.str);
  // Serial.print("finger_sensor_1: ");
  // Serial.println(inData.finger_sensor_1);
  // Serial.print("finger_sensor_2: ");
  // Serial.println(inData.finger_sensor_2);
  // Serial.print("finger_sensor_3: ");
  // Serial.println(inData.finger_sensor_3);
  // Serial.println();
  // sendData();
  messages_send_success = inData.messages_rec;
  messages_recv++;
}

void sendData(){
  // Set values to send
  strcpy(outData.str, "Position Data");
  outData.pos1 = random(1, 255);
  outData.pos2 = random(1, 255);
  outData.pos3 = random(1, 255);
  outData.pos4 = random(1, 255);
  outData.pos5 = random(1, 255);
  outData.pos6 = random(1, 255);
  outData.pos7 = random(1, 255);
  outData.pos8 = random(1, 255);
  outData.pos9 = random(1, 255);
  outData.pos10 = random(1, 255);
  outData.pos11 = random(1, 255);
  outData.pos12 = random(1, 255);
  outData.pos13 = random(1, 255);
  outData.pos14 = random(1, 255);
  outData.pos15 = random(1, 255);
  outData.pos16 = random(1, 255);
  outData.messages_rec = messages_recv;

  // Send struct message via ESP-NOW
  esp_err_t result = esp_now_send(peer_mac, (uint8_t *)&outData, sizeof(outData));

  messages_send_attempt += 1;

  if (result == ESP_OK) {
    // Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
}


void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);

  messages_send_attempt = 0;
  messages_send_success = 0;
  messages_recv = 0;
  i = 0;

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

  // DELAY: enter delay if trying to sync up MAC addresses
  delay(10000);
}

void loop() {
  sendData();
  delay(1000/DATA_RATE);
  i += 1;
  if(i % 200 == 0){
    Serial.println();
    Serial.print("Messages Sent: ");
    Serial.println(messages_send_attempt);
    Serial.print("Messages Delivered: ");
    Serial.println(messages_send_success);
    Serial.print("Success rate: ");
    Serial.print(messages_send_success*100/messages_send_attempt);
    Serial.println(" %");
    Serial.println();
  }
}