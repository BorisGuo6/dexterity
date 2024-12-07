#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_DRV2605.h>

#define HAPTIC_1 6 // low
#define HAPTIC_2 23 // low-medium 
#define HAPTIC_3 19 // medium
#define HAPTIC_4 18 // medium-high
#define HAPTIC_5 27 // high
#define HAPTIC_MAX 118 // max pressure

void setHapticMode(Adafruit_DRV2605 drv_chip, uint8_t mode) {

  drv_chip.setWaveform(0, mode);  // Set effect
  drv_chip.setWaveform(1, 0);     // End waveform
}

void cycleHaptics(Adafruit_DRV2605 drv_chip, uint8_t mode) {

  //Cycle through 6 pressure settings

  mode = HAPTIC_1;  //sharp click - 30%
  Serial.print("Effect #");
  Serial.println(mode);
  setHapticMode(drv_chip, mode);
  for (int i = 0; i < 5; i++) {

    drv_chip.go();
    delay(500);
  }

  mode = HAPTIC_2;  //medium click = 60%
  Serial.print("Effect #");
  Serial.println(mode);
  setHapticMode(drv_chip, mode);
  for (int i = 0; i < 6; i++) {

    drv_chip.go();
    delay(350);
  }

  mode = HAPTIC_3;  //strong click - 60%
  Serial.print("Effect #");
  Serial.println(mode);
  setHapticMode(drv_chip, mode);
  for (int i = 0; i < 8; i++) {

    drv_chip.go();
    delay(250);
  }

  mode = HAPTIC_4;  //strong click - 100%
  Serial.print("Effect #");
  Serial.println(mode);
  setHapticMode(drv_chip, mode);
  for (int i = 0; i < 12; i++) {

    drv_chip.go();
    delay(150);
  }

  mode = HAPTIC_5;  //short double click strong - 100%
  Serial.print("Effect #");
  Serial.println(mode);
  setHapticMode(drv_chip, mode);
  for (int i = 0; i < 20; i++) {

    drv_chip.go();
    delay(150);
  }

  mode = HAPTIC_MAX;  //Long buzz (max pressure)
  Serial.print("Effect #");
  Serial.println(mode);
  setHapticMode(drv_chip, mode);
  for (int i = 0; i < 50; i++) {

    drv_chip.go();
    delay(0);
  }

  delay(5000);
}