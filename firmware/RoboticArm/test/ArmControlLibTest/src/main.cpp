#include <Arduino.h>
#include <FastLED.h>
#include <RoboticArm_ESPNOW.h>
#include <ArmControl.h>

// built in LED
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

#define PEER_MAC {0x54, 0x32, 0x04, 0x89, 0x70, 0x54}

void setup() {
  // setup onboard RGB LED
	FastLED.addLeds<WS2812, PIN_NEOPIXEL, GRB>(leds, NUM_LEDS); // RGB ou GRB ??
	FastLED.setBrightness(RGB_BRIGHTNESS);

  //setup arm Control Library
  armControlSetup();

	//setup ESPNOW
	uint8_t mac[] = PEER_MAC;
	arm_ESPNOWsetup(mac,115200);
}

void loop() {
  controlArm(&arm_inData);
  delay(1000/DATA_RATE);  // Wait before sending again
}