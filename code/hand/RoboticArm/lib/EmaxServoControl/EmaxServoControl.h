#ifndef EMAXSERVOCONTROL_H
#define EMAXSERVOCONTROL_H

#include <Adafruit_PWMServoDriver.h>

#define I2C_SDA     4              // I2C SDA pin
#define I2C_SCL     5               // I2C SCL pin
#define I2C_CLOCK   400000L       // I2C clock rate


// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  170 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  440 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  800 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2220 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SERVO_COUNT 16

// max and min values for finger pos inputs from control glove
#define GLOVE_INPUT_MIN 0
#define GLOVE_INPUT_MAX 240

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
void emaxServoControlSetup();

void controlEmaxServo(uint8_t servo_num, uint16_t pos);

#endif
