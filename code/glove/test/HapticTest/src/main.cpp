#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_DRV2605.h>
#include <Haptic.h>
#include <FastLED.h>
#include <ResponsiveAnalogRead.h>

Adafruit_DRV2605 drv;  // used to interface with the DRV chip
uint8_t effect;    // selected Waveform effect from DRV chip

//define data and clock I2C pins and create I2C bus
#define I2C_SDA 36
#define I2C_SCL 35
TwoWire I2C_LRA = TwoWire(0);

void setup()
{
  
  Serial.begin(115200);

  //Initialize I2C communication
  I2C_LRA.begin(I2C_SDA, I2C_SCL, 400000);

  drv.begin(&I2C_LRA);

  drv.selectLibrary(6);
  drv.setMode(DRV2605_MODE_INTTRIG);
  drv.useLRA();

  //Serial.println("setup"); 

}

void loop()
{
  setup();

  cycleHaptics(drv, effect);


  /*Cycle
  //effect++;
  //if (effect > 118) effect = 1; */

  /*Comparator
  if (effect == 57) effect = 54;
  else if (effect == 54) effect = 52;
  else effect = 57;  */

  }