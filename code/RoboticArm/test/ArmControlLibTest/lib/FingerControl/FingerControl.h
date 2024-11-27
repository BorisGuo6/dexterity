#ifndef FINGERCONTROL_H
#define FINGERCONTROL_H

#include <Arduino.h>
#include "EmaxServoControl.h"

//min and max pulse width for PWM signal
#define MIN_US 800
#define MAX_US 2225

#define MCP_FLEXION_MIN 0
#define MCP_FLEXION_MAX 240
#define MCP_ABDUCTION_MIN -80
#define MCP_ABDUCTION_MAX 80
#define PIP_FLEXION_MIN 0
#define PIP_FLEXION_MAX 255

#define THUMB_CMC_FLEXION_MIN 0 
#define THUMB_CMC_FLEXION_MAX 255
#define THUMB_CMC_ABDUCTION_MIN -125 
#define THUMB_CMC_ABDUCTION_MAX 125
#define THUMB_PIP_FLEXION_MIN 0
#define THUMB_PIP_FLEXION_MAX 255
#define THUMB_ROTATION_MIN 0
#define THUMB_ROTATION_MAX 250

// set up for servos
void fingerControlSetup();

// helper function for calculating metacarpal servo positions based on flexion and abduction
int* metacarpalServoCalc(int MCP_flexion, int MCP_abduction, bool flip_abduction);

// helper function for calculating tip servo positions
int tipServoCalc(int flexion, bool flip_servo);

// code to control finger position based on data from control glove
void controlFingers(uint8_t finger_pos[]);

// control finger
// 0 = index
// 1 = middle
// 2 = ring
// 3 = pinkie
void controlFinger(uint8_t finger_num, uint8_t finger_pos[]);

// move the thumb given the 4 finger_pos data points
void controlThumb(uint8_t finger_pos[]);

// infinite while loop that lets you control a single servo at a time through serial
void moveServoThroughSerial();

//prints positions of the 16 emax servos that control the fingers
void printEmaxServoPositions();


#endif
