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

// Start RoboticArm_ESPNOW.h:

// Create an instance of the struct to be sent/received
// Create it in public space so general arm code can access values
position_packet arm_inData;
haptic_packet arm_outData;

// remove when not monitoring success rate:
int arm_messages_send_attempt;
int arm_messages_send_success;
int arm_messages_rcv;

// general arm code needs to initialize ESPNOW
void arm_ESPNOWsetup(uint8_t board_num);

// general arm code has access to sendData function
void arm_sendData(uint8_t fi, uint8_t fm, uint8_t fr, uint8_t fp, uint8_t ft);

// receive data function will call general arm code

void arm_monitorSuccess();

// End RoboticArm_ESPNOW.h

// Start RoboticArm_ESPNOW.c:

uint8_t peer_mac[6];

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
void ArmOnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  if (len == sizeof(arm_inData)) {
      memcpy(&arm_inData, incomingData, sizeof(arm_inData));
      // Proceed with processing
  } else {
      Serial.println("Received data length does not match expected size.");
  }
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Message: ");
  // Serial.println(inData.str);
  // Serial.print("finger_position_1: ");
  // Serial.println(inData.finger_position_1);
  // Serial.print("finger_angle_1: ");
  // Serial.println(inData.finger_angle_1);
  // Serial.print("wrist_position: ");
  // Serial.println(inData.wrist_position);
  // Serial.print("wrist_angle: ");
  // Serial.println(inData.wrist_angle);
  // Serial.println();
  arm_messages_send_success = arm_inData.messages_rec;
  arm_messages_rcv++;
  pos5_in = arm_inData.finger_pos[5];
  sizeIn = len;
}

void arm_ESPNOWsetup(uint8_t board_num){
  arm_messages_send_attempt = 0;
  arm_messages_send_success = 0;
  arm_messages_rcv = 0;

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

// General Arduino Code for Demonstration

int i;

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  arm_ESPNOWsetup(1);
  i = 0;
}

void loop() {

  arm_sendData(random(1, 100), random(1, 100), random(1, 100), random(1, 100), random(1, 100));

  delay(1000/DATA_RATE);  // Wait before sending again


  i += 1;
  if(i % 200 == 0){
    arm_monitorSuccess();
  }
}

