#include <Arduino.h>
#include <Wire.h>
#include <Haptic.h>
#include <Adafruit_DRV2605.h>
#include <SPI.h>
#include <General_ESPNOW.h>


#define I2C_SDA 35
#define I2C_SCL 0
TwoWire I2C_LRA = TwoWire(0);

Adafruit_DRV2605 drv;

int pressure_reading;

void setup() {
  Serial.begin(115200);

  I2C_LRA.begin(I2C_SDA, I2C_SCL, 400000);

  drv.begin(&I2C_LRA);

  drv.selectLibrary(6);
  drv.setMode(DRV2605_MODE_INTTRIG);
  drv.useLRA();
}

void loop() {
  

   //receive data from Arm ESP
   pressure_reading = NULL;
  
  if(pressure_reading >= 700 & pressure_reading < 1700) {

    setHapticMode(drv, HAPTIC_1);
    drv.go();
  }

  if(pressure_reading >= 1700 & pressure_reading < 2700) {

    setHapticMode(drv, HAPTIC_2);
    drv.go();
  }

  if(pressure_reading >= 2700 & pressure_reading < 3400) {

    setHapticMode(drv, HAPTIC_3);
    drv.go();
  }

  if(pressure_reading >= 3400 & pressure_reading < 3800) {

    setHapticMode(drv, HAPTIC_4);
    drv.go();
  }

  if(pressure_reading >= 3800 & pressure_reading <= 4095) {

    setHapticMode(drv, HAPTIC_5);
    drv.go();
  }
  
  delay(200);
}


