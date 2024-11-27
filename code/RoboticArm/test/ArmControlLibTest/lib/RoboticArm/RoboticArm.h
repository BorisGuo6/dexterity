#ifndef ROBOTIC_ARM_H
#define ROBOTIC_ARM_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"

#define PEER_MAC {0x54, 0x32, 0x04, 0x89, 0x70, 0x54}
#define ENABLE_PRESSURE_PRINT 0
#define ENABLE_CONTROL_PRINT 1 

void pressureSensorCode(void* params);
void armControlCode(void* params);

#endif