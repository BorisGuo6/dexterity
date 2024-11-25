#ifndef HALL_EFFECT_SENSORS_H
#define HALL_EFFECT_SENSORS_H

#include <Arduino.h>
#include <stdint.h>
#include <ResponsiveAnalogRead.h>

//TODO define multiplexer input pins
#define S0 1
#define S1 2
#define S2 3
#define S3 4

#define SENSOR_COUNT 16

#define MCP_FLEXION_MIN 0
#define MCP_FLEXION_MAX 240

#define PIP_FLEXION_MIN 0
#define PIP_FLEXION_MAX 255

#define MCP_ABDUCTION_MIN -80
#define MCP_ABDUCTION_MAX 80

#define THUMB_CMC_FLEXION_MIN 0 
#define THUMB_CMC_FLEXION_MAX 255

#define THUMB_CMC_ABDUCTION_MIN -125
#define THUMB_CMC_ABDUCTION_MAX 125

#define THUMB_PIP_FLEXION_MIN 0
#define THUMB_PIP_FLEXION_MAX 255

/**
 * Initializes the Hall effect sensors.
 */
void hallEffectSensorsSetup();

/**
 * Reads the raw angle values, adjusts them, and stores them in the angles array. Calling this function requires
 * initialize() to have already been called.
 */
void updateAngles();

/**
 * Prints the contents of the angles array over Serial
 */
void printAngles();

/**
 * Sends the current angle data to the robotic hand. Calling this function requires an ESPNow
 * connection to have already been established with the hand.
 */
void sendData();



void initializeCalibrationValues();
void measureAngles();
void calibrate();
void adjustAngles();
int32_t adjustMCPAbductionAngle(int32_t i);
int32_t adjustMCPFlexionAngle(int32_t i);
int32_t adjustPIPFlexionAngle(int32_t i);
int32_t adjustThumbCMCFlexionAngle(int32_t i);
int32_t adjustThumbCMCAbductionAngle(int32_t i);
int32_t adjustThumbPIPFlexionAngle(int32_t i);

float poly(double x, double a,double b,double c);

#endif