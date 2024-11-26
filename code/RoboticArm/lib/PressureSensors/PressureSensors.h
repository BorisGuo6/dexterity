#ifndef PRESSURE_SENSORS_H
#define PRESSURE_SENSORS_H

#include "Arduino.h"
#include "RoboticArm_ESPNOW.h"

void pollPressureSensors();
void sendPressureData();

#endif