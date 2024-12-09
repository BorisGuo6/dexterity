#ifndef EMAXSERVOCONTROL_H
#define EMAXSERVOCONTROL_H

#include <Adafruit_PWMServoDriver.h>
#include "hand_pins.h"

#define I2C_SDA     DIGITAL_SERVO_SDA              // I2C SDA pin
#define I2C_SCL     DIGITAL_SERVO_SCL              // I2C SCL pin
#define I2C_CLOCK   400000L       // I2C clock rate


// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  170 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  425 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  800 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2220 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SERVO_COUNT 16

// max and min values for finger pos inputs from control glove
#define GLOVE_INPUT_MIN 0
#define GLOVE_INPUT_MAX 255


//IM0 = Index Metacarpal 0 (0 is next to fingers, 1 is next to wrist)
//IT = Index Tip

#define SERVO_COUNT 16

//servo pins on the PWM board

//Index
#define IM0_SERVO_PIN 13
#define IM1_SERVO_PIN 12
#define IT_SERVO_PIN 7

//Middle
#define MM0_SERVO_PIN 15
#define MM1_SERVO_PIN 14
#define MT_SERVO_PIN 6

//Ring
#define RM0_SERVO_PIN 9
#define RM1_SERVO_PIN 8
#define RT_SERVO_PIN 5

//Pinkie
#define PM0_SERVO_PIN 10
#define PM1_SERVO_PIN 11
#define PT_SERVO_PIN 4

//Pinkie
#define PM0_SERVO_PIN 10
#define PM1_SERVO_PIN 11
#define PT_SERVO_PIN 4

//Thumb
#define TM0_SERVO_PIN 1
#define TM1_SERVO_PIN 2
#define TT_SERVO_PIN 3
#define TROT_SERVO_PIN 0

// servo positions are stored here for easy printing
extern int16_t all_servo_positions[SERVO_COUNT];

//number to keep track of position in all_servo_positions array
extern int16_t servo_position_num;

// set up for servos
void emaxServoControlSetup();

void controlEmaxServo(uint8_t servo_num, uint16_t pos);

#endif
