#include <esp_now.h>
#include <WiFi.h>
#include <stdint.h>

// General_ESPNOW.h: 

#define ESPNOW_WIFI_CHANNEL 5            // WiFi channel to be used by ESP-NOW. The available channels will depend on your region.
#define ESPNOW_WIFI_MODE    WIFI_STA     // WiFi mode to be used by ESP-NOW. Any mode can be used.
#define ESPNOW_WIFI_IF      WIFI_IF_STA  // WiFi interface to be used by ESP-NOW. Any interface can be used.
#define DATA_RATE           100          // In Hz
#define SYNC_DELAY          10           // delay (in sec) between setting up ESPNOW and sending packets
#define PEER_MAC_1          {0x3C, 0x84, 0x27, 0x14, 0x7B, 0xB0} // MAC for board 1
#define PEER_MAC_2          {0x3C, 0x84, 0x27, 0xE1, 0xB3, 0x8C} // MAC for board 2

// Define the data packets
typedef struct position_packet {
  // remove when not monitoring success rate:
  int messages_rec;
  // end remove
  uint8_t finger_pos[16];
  uint8_t wrist_pos[3];
  uint8_t arm_pos[3];
} position_packet;

typedef struct haptic_packet {
  // remove when not monitoring success rate:
  int messages_rec;
  // end remove
  uint8_t force_index;
  uint8_t force_middle;
  uint8_t force_ring;
  uint8_t force_pinky;
  uint8_t force_thumb;
} haptic_packet;

// End General_ESPNOW.h

// Start HapticGlove_ESPNOW.h: 

// Create an instance of the struct to be sent/received
// Create it in public space so general glove code can access values
position_packet glove_outData;
haptic_packet glove_inData;

// remove when not monitoring success rate:
int glove_messages_send_attempt;
int glove_messages_send_success;
int glove_messages_rcv;

// general glove code needs to initialize ESPNOW
void glove_ESPNOWsetup(uint8_t board_num);

// general glove code has access to sendData function
void glove_sendData(uint8_t fpos[], uint8_t wpos[], uint8_t apos[]);

// receive data function will call general arm code

void glove_monitorSuccess();

// End HapticGlove_ESPNOW.h

// Start HapticGlove_ESPNOW.c:

uint8_t peer_mac[6];

// Function to handle the result of data send
void GloveOnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback function that will be executed when data is received
void GloveOnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  if (len == sizeof(glove_inData)) {
      memcpy(&glove_inData, incomingData, sizeof(glove_inData));
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
  glove_messages_send_success = glove_inData.messages_rec;
  glove_messages_rcv++;
}

void glove_ESPNOWsetup(uint8_t board_num){
  glove_messages_send_attempt = 0;
  glove_messages_send_success = 0;
  glove_messages_rcv = 0;

  switch(board_num){
    case(1): {
      uint8_t temp[] = PEER_MAC_1;
      for(int j=0; j<6; j++){
        peer_mac[j] = temp[j];
      }
      break;
    }
    case(2): {
      uint8_t temp[] = PEER_MAC_2;
      for(int j=0; j<6; j++){
        peer_mac[j] = temp[j];
      }
      break;
    }
    deafult:
      Serial.print("ERROR: Board num not recognized in arm_ESPNOWsetup. Board received: ");
      Serial.println(board_num);
      while(1) delay(10000);
  }

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
  esp_now_register_send_cb(GloveOnDataSent);

  // Register the receive callback function
  esp_now_register_recv_cb(GloveOnDataRecv);

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

void glove_sendData(uint8_t fpos[], uint8_t wpos[], uint8_t apos[]){
  // Set values to send
  for(int j=0; j<16; j++){
    glove_outData.finger_pos[j] = fpos[j];
  }
  for(int j=0; j<3; j++){
    glove_outData.wrist_pos[j] = wpos[j];
    glove_outData.arm_pos[j] = apos[j];
  }
  glove_outData.messages_rec = glove_messages_rcv;

  // Send struct message via ESP-NOW
  esp_err_t result = esp_now_send(peer_mac, (uint8_t *)&glove_outData, sizeof(glove_outData));

  glove_messages_send_attempt += 1;

  if (result == ESP_OK) {
    // Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
}

void glove_monitorSuccess(){
  Serial.println();
  Serial.print("Messages Sent: ");
  Serial.println(glove_messages_send_attempt);
  Serial.print("Messages Delivered: ");
  Serial.println(glove_messages_send_success);
  Serial.print("Success rate: ");
  Serial.print(glove_messages_send_success*100/glove_messages_send_attempt);
  Serial.println(" %");
  Serial.println();
}

// End HapticGlove_ESPNOW.c

// General Arduino Code for Demonstration

int i;

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  glove_ESPNOWsetup(2);
  i = 0;
}

void loop() {
  uint8_t fpos[16];
  uint8_t wpos[3];
  uint8_t apos[3];
  for(int j=0; j<16; j++){
    fpos[j] = random(1, 255);
  }
  for(int j=0; j<3; j++){
    wpos[j] = random(1, 255);
    apos[j] = random(1, 255);
  }
  glove_sendData(fpos, wpos, apos);
  delay(1000/DATA_RATE);
  i += 1;
  if(i % 200 == 0){
    glove_monitorSuccess();
  }
}