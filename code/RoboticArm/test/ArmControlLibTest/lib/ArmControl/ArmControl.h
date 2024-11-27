#ifndef ARM_CONTROL_H
#define ARM_CONTROL_H

#include <Arduino.h>
#include <FastLED.h>
#include <FingerControl.h>
#include <WristControl.h>
#include <General_ESPNOW.h>

// built in LED
#define NUM_LEDS 1

void armControlSetup();
void controlArm(position_packet* arm_inData);

#endif