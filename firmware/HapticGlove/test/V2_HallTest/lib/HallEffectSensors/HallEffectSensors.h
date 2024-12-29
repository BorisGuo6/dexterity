#ifndef HALL_EFFECT_SENSORS_H
#define HALL_EFFECT_SENSORS_H

#include <Arduino.h>
#include <stdint.h>
#include <ResponsiveAnalogRead.h>
#include <ControlGlovePins.h>

//TODO define multiplexer input pins
#define S0 HALL_S0
#define S1 HALL_S1
#define S2 HALL_S2
#define S3 HALL_S3

#define SENSOR_COUNT 16

extern int32_t rawVals[SENSOR_COUNT];
extern float proto_angles[SENSOR_COUNT];
extern float min_angles[SENSOR_COUNT];
extern float max_angles[SENSOR_COUNT];

/**
 * Initializes the Hall effect sensors.
 */
void hallEffectSensorsSetup();

/**
 * Sends the current angle data to the robotic hand. Calling this function requires an ESPNow
 * connection to have already been established with the hand.
 */
void sendData();

/**
 * Reads the raw angle values, adjusts them, and stores them in the proto_angles array
 */
void measureHallEffectSensors();
void calibrateHallEffectSensors();

void reorderArray(int32_t arr[16]);

float poly(double x, double a,double b,double c);

#endif