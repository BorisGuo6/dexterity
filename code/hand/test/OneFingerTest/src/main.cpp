#include <Arduino.h>
#include <FastLED.h>
#include <ESP32Servo.h>
#include <RoboticArm_ESPNOW.h>

// built in LED
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

#define SERVO_MIN 0
#define SERVO_MAX 160

// create four servo objects
Servo servo0;
Servo servo1;

// Published values for SG90 servos; adjust if needed
int minUs = 800;
int maxUs = 2225;

int servo1Pin = 1;
int servo2Pin = 2;

int pos = 0; // position in degrees
ESP32PWM pwm;
void setup()
{

	Serial.begin(115200);
	// setup onboard RGB LED
	FastLED.addLeds<WS2812, PIN_NEOPIXEL, GRB>(leds, NUM_LEDS); // RGB ou GRB ??
	FastLED.setBrightness(RGB_BRIGHTNESS);

	//setup ESP32
	arm_ESPNOWsetup(1);

	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

	servo0.setPeriodHertz(50); // Standard 50hz servo
	servo1.setPeriodHertz(50); // Standard 50hz servo
							   // servo3.setPeriodHertz(330);      // Standard 50hz servo
							   // servo4.setPeriodHertz(200);      // Standard 50hz servo
							   // servo5.setPeriodHertz(50);      // Standard 50hz servo

	servo0.attach(servo1Pin, minUs, maxUs);
	servo1.attach(servo2Pin, minUs, maxUs);
}

void printAngles() {
  for (uint8_t i = 0; i < 16; i++){
    Serial.print(">Joint_");
    Serial.print(i);
    Serial.print(":");
    Serial.println(arm_inData.finger_pos[i]);
  }
}

void servoWrite(int servoNum, int angle){
	if(servoNum == 0){
		servo0.write(180-angle);
	}else{
		servo1.write(angle);
	}
}

void loop()
{

	//printAngles();

	int joint_10 = arm_inData.finger_pos[10]; // MCP flexion
	int joint_9 = arm_inData.finger_pos[9]; // MCP abduction

	//hardcoded values
	int converted_flexion_angle = (int)((double)(joint_10 - 33) / 107.0 * SERVO_MAX);
	int converted_abduction_angle = (joint_9 - 71)*2;

	//servo offset for abduction
	int servo0_offset = 0;
	int servo1_offset = 0;

	if(converted_abduction_angle < 0){ //right
		converted_abduction_angle = max(converted_abduction_angle,-40);
		servo1_offset=-converted_abduction_angle;
	}else{ //left
		converted_abduction_angle = min(converted_abduction_angle,40);
		servo0_offset=converted_abduction_angle;
	}

	if(converted_flexion_angle < SERVO_MIN){
		converted_flexion_angle = SERVO_MIN;
	}else if (converted_flexion_angle > SERVO_MAX){
		converted_flexion_angle = SERVO_MAX;
	}

	Serial.print(converted_flexion_angle);
	Serial.print(" ");
	Serial.println(converted_abduction_angle);

	servoWrite(0,converted_flexion_angle+servo0_offset);
	servoWrite(1,converted_flexion_angle+servo1_offset);


	// for (pos = 0; pos <= 160; pos += 1) { // sweep from 0 degrees to 180 degrees
	// 	// in steps of 1 degree
	// 	servo0.write(180-pos);
	//   servo1.write(pos);
	//   Serial.println(pos);
	// 	delay(1);             // waits 20ms for the servo to reach the position
	// }
	// servo0.write(180 - 160);
	// servo1.write(160);
	// delay(1000);
	// for (pos = 160; pos >= 0; pos -= 1) { // sweep from 180 degrees to 0 degrees
	// 	servo1.write(180-pos);
	//   servo2.write(pos);
	//   Serial.println(pos);
	// 	delay(1);
	// }
	// servo0.write(180);
	// servo1.write(0);
	// delay(1000);

	// servo1.detach();
	// servo2.detach();

	delay(1000/DATA_RATE);  // Wait before sending again

}
