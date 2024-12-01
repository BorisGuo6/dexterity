#ifndef WRIST_CONTROL_H
#define WRIST_CONTROL_H

#include "Arduino.h"
#include "RoboticArm.h"

void wristControlSetup();
void controlWrist(uint32_t* wrist_pos);

#endif