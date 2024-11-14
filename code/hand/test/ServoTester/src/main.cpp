#include <Arduino.h>
#include <FastLED.h>
#include <ESP32Servo.h>

// built in LED
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

// create four servo objects
Servo servo0;
// Servo servo1;

// Published values for SG90 servos; adjust if needed
int minUs = 800;
int maxUs = 2225;

int servo1Pin = 10;
// int servo2Pin = 2;

int pos = 0; // position in degrees
ESP32PWM pwm;
void setup()
{

	Serial.begin(115200);
	// setup onboard RGB LED
	FastLED.addLeds<WS2812, PIN_NEOPIXEL, GRB>(leds, NUM_LEDS); // RGB ou GRB ??
	FastLED.setBrightness(RGB_BRIGHTNESS);

	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

	servo0.setPeriodHertz(50); // Standard 50hz servo
	//servo1.setPeriodHertz(50); // Standard 50hz servo
							   // servo3.setPeriodHertz(330);      // Standard 50hz servo
							   // servo4.setPeriodHertz(200);      // Standard 50hz servo
							   // servo5.setPeriodHertz(50);      // Standard 50hz servo

	servo0.attach(servo1Pin, minUs, maxUs);
	//servo1.attach(servo2Pin, minUs, maxUs);
}

void loop() {
  Serial.println("Enter position: ");
  while(Serial.available() == 0){};
  pos = Serial.readString().toInt();
  if (pos >= 0 && pos <= 180){
    servo0.write(pos);
    Serial.print("Turned to: ");
    Serial.println(pos);
  }
  else {
    Serial.println("Invalid postition!");
  }
}