#include <Arduino.h>

#include <HapticGlove_ESPNOW.h>
#include <HallEffectSensors.h>

#define PEER_MAC {0x3C, 0x84, 0x27, 0x14, 0x7B, 0xB0}

void setup() {
	uint8_t mac[] = PEER_MAC;
	glove_ESPNOWsetup(mac, 115200);
	hallEffectSensorsSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  updateAngles();
  printAngles();
}
