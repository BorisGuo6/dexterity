#ifndef ROBOTIC_ARM_H
#define ROBOTIC_ARM_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"

//{0x3C, 0x84, 0x27, 0xE1, 0xB3, 0x8C}
//{0x54, 0x32, 0x04, 0x89, 0x70, 0x54}
#define PEER_MAC {0x54, 0x32, 0x04, 0x89, 0x70, 0x54}
#define ENABLE_PRESSURE_PRINT 0
#define ENABLE_CONTROL_PRINT 1 
#define ENABLE_ESPNOW_PRINT 1
#define ISR0_FREQ 50 // in Hz
#define ISR1_FREQ 50 // in Hz

void pressureSensorCode(void* params);
void armControlCode(void* params);

#endif