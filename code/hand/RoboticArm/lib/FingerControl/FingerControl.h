#ifndef FINGERCONTROL_H
#define FINGERCONTROL_H

#include <ESP32Servo.h>

//min and max pulse width for PWM signal
#define MIN_US 800
#define MAX_US 2225

#define MCP_FLEXION_MIN 0
#define MCP_FLEXION_MAX 160
#define MCP_ABDUCTION_MIN -40
#define MCP_ABDUCTION_MAX 40

//IM0 = Index Metacarpal 0 (0 is next to fingers, 1 is next to wrist)
//IT = Index Tip
#define IM0_SERVO_PIN 4
#define IM1_SERVO_PIN 5
#define IT_SERVO_PIN 6

//Middle
#define MM0_SERVO_PIN 7
#define MM1_SERVO_PIN 15
#define MT_SERVO_PIN 16

//Ring
#define RM0_SERVO_PIN 17
#define RM1_SERVO_PIN 18
#define RT_SERVO_PIN 8


//Pinkie
#define PM0_SERVO_PIN 3
#define PM1_SERVO_PIN 10
#define PT_SERVO_PIN 11


// set up for servos
void fingerControlSetup();

// helper function for calculating metacarpal servo positions based on flexion and abduction
int* metacarpalServoCalc(int MCP_flexion, int MCP_abduction, bool flip_M0_servo, bool flip_M1_servo);

// helper function for calculating tip servo positions
int tipServoCalc(int flexion, bool flip_servo);

// code to control finger position based on data from control glove
void controlFingers(uint8_t finger_pos[]);

#endif
