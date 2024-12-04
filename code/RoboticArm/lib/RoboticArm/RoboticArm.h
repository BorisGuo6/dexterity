#ifndef ROBOTIC_ARM_H
#define ROBOTIC_ARM_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Arduino.h"

//{0x3C, 0x84, 0x27, 0xE1, 0xB3, 0x8C}
//{0x54, 0x32, 0x04, 0x89, 0x70, 0x54}
// #define PEER_MAC {0x54, 0x32, 0x04, 0x89, 0x70, 0x54}
//#define PEER_MAC {0x3C, 0x84, 0x27, 0xE1, 0xCE, 0x5C}
//                  3C   :84   :27   :14   :7B   :80
// #define PEER_MAC {0x3C, 0x84, 0x27, 0x14, 0x7B, 0x80}
// #define PEER_MAC {0x24, 0xEC, 0x4A, 0x45, 0x23, 0x80}
#define PEER_MAC {0x3C, 0x84, 0x27, 0x14, 0x7B, 0xB0}
#define ENABLE_PRESSURE_PRINT 1
#define ENABLE_CONTROL_PRINT 0 
#define ENABLE_WRIST_PRINT 0
#define ENABLE_ESPNOW_PRINT 0
#define ISR0_FREQ 50 // in Hz
#define ISR1_FREQ 50 // in Hz
#define SERVO_ID_1 1
#define SERVO_ID_2 2
#define SERVO_ID_3 3
#define SERVO_ID_4 4
#define RX_PIN_2 18
#define TX_PIN_2 17

void pressureSensorCode(void* params);
void armControlCode(void* params);

#endif