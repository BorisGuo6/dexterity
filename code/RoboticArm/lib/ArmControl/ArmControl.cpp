#include <ArmControl.h>

// built in LED
CRGB leds[NUM_LEDS];
int i;


void armControlSetup() {
  // setup onboard RGB LED
	FastLED.addLeds<WS2812, PIN_NEOPIXEL, GRB>(leds, NUM_LEDS); // RGB ou GRB ??
	FastLED.setBrightness(RGB_BRIGHTNESS);

  //setup Finger Control Library
  fingerControlSetup();
  wristControlSetup();
  i = 1;

  Serial.println("ARM CONTROL: Missing dependencies for ArmControlPanel, WristControl");
}

void controlArm(){
  controlFingers(arm_inData.finger_pos);
  controlWrist(arm_inData.wrist_pos);
  if(ENABLE_CONTROL_PRINT){
    Serial.print("controlArm data received in core ");
    Serial.println(xPortGetCoreID());
  }
  if(i % 200 == 0 && ENABLE_ESPNOW_PRINT){
    arm_monitorSuccess();
    i = 1;
  }
  i += 1;
}