#include <Arduino.h>
#include <FastLED.h>
#include <ESP32Servo.h>
#include <RoboticArm_ESPNOW.h>

// built in LED
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

#define MCP_FLEXION_MIN 0
#define MCP_FLEXION_MAX 160

#define MCP_ABDUCTION_MIN -40
#define MCP_ABDUCTION_MAX 40

#define PEER_MAC {0x54, 0x32, 0x04, 0x89, 0x70, 0x54}

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

	//Serial.begin(115200);
	// setup onboard RGB LED
	FastLED.addLeds<WS2812, PIN_NEOPIXEL, GRB>(leds, NUM_LEDS); // RGB ou GRB ??
	FastLED.setBrightness(RGB_BRIGHTNESS);

	//setup ESPNOW
	uint8_t mac[] = PEER_MAC;
	arm_ESPNOWsetup(mac,115200);

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

	int MCP_flexion = arm_inData.finger_pos[10]; // MCP flexion
	int MCP_abduction = arm_inData.finger_pos[9]+MCP_ABDUCTION_MIN; // MCP abduction

	//servo offset for abduction
	int servo0_offset = 0;
	int servo1_offset = 0;

	if(MCP_abduction < 0){ //right
		MCP_abduction = max(MCP_abduction,MCP_ABDUCTION_MIN);
		servo1_offset=-MCP_abduction;
	}else{ //left
		MCP_abduction = min(MCP_abduction,MCP_ABDUCTION_MAX);
		servo0_offset=MCP_abduction;
	}


	Serial.print(MCP_flexion);
	Serial.print(" ");
	Serial.println(MCP_abduction);

	int servo0_pos = constrain(MCP_flexion+servo0_offset,MCP_FLEXION_MIN,MCP_FLEXION_MAX);
	int servo1_pos = constrain(MCP_flexion+servo1_offset,MCP_FLEXION_MIN,MCP_FLEXION_MAX);


	servoWrite(0,servo0_pos);
	servoWrite(1,servo1_pos);


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
