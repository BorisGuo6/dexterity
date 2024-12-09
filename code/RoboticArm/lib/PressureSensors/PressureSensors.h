#ifndef PRESSURE_SENSORS_H
#define PRESSURE_SENSORS_H

#include "RoboticArm.h"
#include "RoboticArm_ESPNOW.h"

void setupPressureSensors();
void pollPressureSensors();
void sendPressureData();

#endif