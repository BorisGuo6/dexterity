#ifndef HAPTIC_GLOVE_H
#define HAPTIC_GLOVE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "haptic_glove_pins.h"
#include "Arduino.h"
#include "GloveControlPanel.h"

// #define PEER_MAC {0x54, 0x32, 0x04, 0x89, 0x70, 0x54}
//                  3C:   84:   27:   14:   7B:   B0
// #define PEER_MAC {0x3C, 0x84, 0x27, 0x14, 0x7B, 0xB0}
#define PEER_MAC {0x24, 0xEC, 0x4A, 0x45, 0x23, 0x80}
#define IMUS_CONNECTED 1
#define ENABLE_HAPTICS 0
#define ENABLE_HAPTICS_PRINT 0
#define ENABLE_SENSORS_PRINT 0
#define ENABLE_IMU_SENSORS_PRINT 1
#define ENABLE_ESPNOW_PRINT 1
#define TRACK_ISR_0 0
#define TRACK_ISR_1 0
#define ISR0_FREQ 50 // in Hz
#define ISR1_FREQ 50 // in Hz

extern GloveControlPanel controlPanel;
extern volatile bool button_pressed;

void sensorProcessingCode(void* params);
void hapticControlCode(void* params);

#endif