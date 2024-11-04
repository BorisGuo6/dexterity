#ifndef FINGERCONTROL_H
#define FINGERCONTROL_H

#include <ESP32Servo.h>

//min and max times for PWN signal to be high
#define MIN_US 800
#define MAX_US 2225

#define MCP_FLEXION_MIN 0
#define MCP_FLEXION_MAX 160
#define MCP_ABDUCTION_MIN -40
#define MCP_ABDUCTION_MAX 40

//IM0 = Index Metacarpal 0
#define IM0_SERVO_PIN 1
#define IM1_SERVO_PIN 2

//Middle
#define MM0_SERVO_PIN 41
#define MM1_SERVO_PIN 40

//Ring
#define RM0_SERVO_PIN 38
#define RM1_SERVO_PIN 37

//Pinkie
#define PM0_SERVO_PIN 47
#define PM1_SERVO_PIN 21

Servo IM0_servo;
Servo IM1_servo;

Servo MM0_servo;
Servo MM1_servo;

Servo RM0_servo;
Servo RM1_servo;

Servo PM0_servo;
Servo PM1_servo;

ESP32PWM pwm;

// set up for servos
void fingerControlSetup();

// code to control finger position based on data from control glove
void controlFingers(uint8_t finger_pos[]);

#endif
