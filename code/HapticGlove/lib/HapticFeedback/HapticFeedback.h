#ifndef HAPTIC_FEEDBACK_H
#define HAPTIC_FEEDBACK_H

#include "HapticGlove.h"
#include "HapticGlove_ESPNOW.h"
#include <Wire.h>
#include <Haptic.h>
#include <Adafruit_DRV2605.h>
#include <SPI.h>

#define I2C_SDA 35
#define I2C_SCL 0

void setupFeedback();
void triggerFeedback();

#endif