#include <Arduino.h>
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

//array of test pins
uint8_t test_pins[5] = {TEST0,TEST1,TEST2,TEST3,TEST4};

// uncomment for ADC testing
// ResponsiveAnalogRead analog0(TEST0, true);
// ResponsiveAnalogRead analog1(TEST1, true);
// ResponsiveAnalogRead analog2(TEST2, true);


void setup()
{
  Serial.begin(115200);
  //setup onboard RGB LED
  FastLED.addLeds<WS2812, PIN_NEOPIXEL, GRB>(leds, NUM_LEDS); // RGB ou GRB ??
  FastLED.setBrightness(RGB_BRIGHTNESS);
  Serial.println("setup");

  //setup test pins as digital outputs
  for (uint8_t i=0; i<5;i++){
    pinMode(test_pins[i], OUTPUT);
  }  
}
void loop()
{
  leds[0] = CRGB(random(0, 255),random(0, 255),random(0, 255));
  FastLED.show();

  // LED cycle loop

  // leds[0] = CRGB::Green;
  // FastLED.show();
  // Serial.println("green");
  // delay(500);
  // leds[0] = CRGB::Blue;
  // FastLED.show();
  // Serial.println("blue");
  // delay(500);
  // leds[0] = CRGB::Black;
  // FastLED.show();
  // delay(500);

  //testing digital of pins

  for (uint8_t i=0; i<5;i++){
    digitalWrite(test_pins[i], HIGH);
  }
  delay(500);
  for (uint8_t i=0; i<5;i++){
    digitalWrite(test_pins[i], LOW);
  }
  delay(500);

  //3 analog channel test
  //the prints are set up to work with Teleplot
  
  // analog0.update();
  // Serial.print(">a0r:");
  // Serial.println(analog0.getRawValue());
  // Serial.print(">a0v:");
  // Serial.println(analog0.getValue());

  // analog1.update();
  // Serial.print(">a1r:");
  // Serial.println(analog1.getRawValue());
  // Serial.print(">a1v:");
  // Serial.println(analog1.getValue());

  // analog2.update();
  // Serial.print(">a2r:");
  // Serial.println(analog2.getRawValue());
  // Serial.print(">a2v:");
  // Serial.println(analog2.getValue());

  //delay(50);

}