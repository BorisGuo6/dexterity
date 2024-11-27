#ifndef ARM_CONTROL_H
#define ARM_CONTROL_H

#include "RoboticArm.h"
#include <FastLED.h>
#include <FingerControl.h>
#include <WristControl.h>
#include <RoboticArm_ESPNOW.h>

// built in LED
#define NUM_LEDS 1

void armControlSetup();
void controlArm();

#endif