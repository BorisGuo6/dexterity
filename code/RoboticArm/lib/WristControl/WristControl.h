#ifndef WRIST_CONTROL_H
#define WRIST_CONTROL_H

#include "Arduino.h"
#include "RoboticArm.h"
#include "SCServo.h"

void wristControlSetup();
void controlWrist(float* wrist_pos);

#endif