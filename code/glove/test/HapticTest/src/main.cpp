#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_DRV2605.h>
#include <FastLED.h>
#include <ResponsiveAnalogRead.h>

#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

//5 test pins
#define TEST0 0
#define TEST1 3
#define TEST2 45
#define TEST3 46
#define TEST4 1

Adafruit_DRV2605 drv;  // used to interface with the DRV chip
uint8_t effect;    // selected Waveform effect from DRV chip

//define data and clock I2C pins and create I2C bus
#define I2C_SDA 20
#define I2C_SCL 21
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

  Serial.println("setup"); 

}

void loop()
{
  setup();

  //Cycle through 6 pressure settings

  effect = 6;  //sharp click - 30%
  Serial.print("Effect #");
  Serial.println(effect);
  drv.setWaveform(0, effect);  // Set effect
  drv.setWaveform(1, 0);       // End waveform
  for (int i = 0; i < 5; i++) {

    drv.go();
    delay(500);
  }

  effect = 23;  //medium click = 60%
  Serial.print("Effect #");
  Serial.println(effect);
  drv.setWaveform(0, effect);  // Set effect
  drv.setWaveform(1, 0);       // End waveform
  for (int i = 0; i < 6; i++) {

    drv.go();
    delay(350);
  }

  effect = 19;  //strong click - 60%
  Serial.print("Effect #");
  Serial.println(effect);
  drv.setWaveform(0, effect);  // Set effect
  drv.setWaveform(1, 0);       // End waveform
  for (int i = 0; i < 8; i++) {

    drv.go();
    delay(250);
  }

  effect = 18;  //strong click - 100%
  Serial.print("Effect #");
  Serial.println(effect);
  drv.setWaveform(0, effect);  // Set effect
  drv.setWaveform(1, 0);       // End waveform
  for (int i = 0; i < 12; i++) {

    drv.go();
    delay(150);
  }

  effect = 27;  //short double click strong - 100%
  Serial.print("Effect #");
  Serial.println(effect);
  drv.setWaveform(0, effect);  // Set effect
  drv.setWaveform(1, 0);       // End waveform
  for (int i = 0; i < 20; i++) {

    drv.go();
    delay(150);
  }

  effect = 118;  //Long buzz (max pressure)
  Serial.print("Effect #");
  Serial.println(effect);
  drv.setWaveform(0, effect);  // Set effect
  drv.setWaveform(1, 0);       // End waveform
  for (int i = 0; i < 50; i++) {

    drv.go();
    delay(0);
  }


  // Pause for differentiation between effects
  delay(5000);

  /*Cycle
  //effect++;
  //if (effect > 118) effect = 1; */

  /*Comparator
  if (effect == 57) effect = 54;
  else if (effect == 54) effect = 52;
  else effect = 57;  */

  }
