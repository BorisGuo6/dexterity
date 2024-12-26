#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_DRV2605.h>

#define HAPTIC_1 6 // low
#define HAPTIC_2 23 // low-medium 
#define HAPTIC_3 19 // medium
#define HAPTIC_4 18 // medium-high
#define HAPTIC_5 27 // high
#define HAPTIC_MAX 118 // max pressure


void setHapticMode(Adafruit_DRV2605 drv_chip, uint8_t mode);

void cycleHaptics(Adafruit_DRV2605 drv_chip, uint8_t mode);
