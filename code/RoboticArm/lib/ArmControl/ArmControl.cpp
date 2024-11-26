#include <ArmControl.h>

// built in LED
CRGB leds[NUM_LEDS];


void armControlSetup() {
  // setup onboard RGB LED
	FastLED.addLeds<WS2812, PIN_NEOPIXEL, GRB>(leds, NUM_LEDS); // RGB ou GRB ??
	FastLED.setBrightness(RGB_BRIGHTNESS);

  //setup Finger Control Library
  fingerControlSetup();
  wristControlSetup();

  Serial.println("ARM CONTROL: Missing dependencies for ArmControlPanel, WristControl");
}

void controlArm(position_packet* arm_inData){
  controlFingers(arm_inData->finger_pos);
  controlWrist(arm_inData->wrist_pos);
}