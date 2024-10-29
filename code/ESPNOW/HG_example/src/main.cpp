#include <HapticGlove_ESPNOW.h>
// General Arduino Code for Demonstration

int i;

void setup() {
  // Start the Serial Monitor
  uint8_t mac[] = PEER_MAC_2;
  int baud_rate = 115200;
  glove_ESPNOWsetup(mac, baud_rate);
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
