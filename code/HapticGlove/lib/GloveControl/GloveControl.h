#ifndef GLOVE_CONTROL_H
#define GLOVE_CONTROL_H

#include "HapticGlove.h"
#include "HapticGlove_ESPNOW.h"
#include "FingerTracking.h"
#include "HallEffectSensors.h"

void gloveControlSetup();
void sendPositionData();

#endif