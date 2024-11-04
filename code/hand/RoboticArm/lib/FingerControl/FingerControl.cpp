#include "FingerControl.h"

Servo IM0_servo;
Servo IM1_servo;

Servo MM0_servo;
Servo MM1_servo;

Servo RM0_servo;
Servo RM1_servo;

Servo PM0_servo;
Servo PM1_servo;

ESP32PWM pwm;

void fingerControlSetup(){
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    IM0_servo.setPeriodHertz(50); // Standard 50hz servo
    IM1_servo.setPeriodHertz(50); // Standard 50hz servo

    IM0_servo.attach(IM0_SERVO_PIN, MIN_US, MAX_US);
    IM1_servo.attach(IM1_SERVO_PIN, MIN_US, MAX_US);
}

int* metacarpalServoCalc(int MCP_flexion, int MCP_abduction, bool flip_M0_servo, bool flip_M1_servo){
    //servo offset for abduction
	int servo0_offset = 0;
	int servo1_offset = 0;

    //calculate how much the servos should offset from each other for abduction
	if(MCP_abduction < 0){ //right
		MCP_abduction = max(MCP_abduction,MCP_ABDUCTION_MIN);
		servo1_offset=-MCP_abduction;
	}else{ //left
		MCP_abduction = min(MCP_abduction,MCP_ABDUCTION_MAX);
		servo0_offset=MCP_abduction;
	}

    //constrain to limits
	int servo0_pos = constrain(MCP_flexion+servo0_offset,MCP_FLEXION_MIN,MCP_FLEXION_MAX);
	int servo1_pos = constrain(MCP_flexion+servo1_offset,MCP_FLEXION_MIN,MCP_FLEXION_MAX);

    //flip servos if needed
    if(flip_M0_servo){
        servo0_pos=180-servo0_pos;
    }
    if(flip_M1_servo){
        servo1_pos=180-servo1_pos;
    }

    //allocate memeory for servo position array
    int* servo_positions = new int[2];
    servo_positions[0] = servo0_pos;
    servo_positions[1] = servo1_pos;

    return servo_positions;
}

void controlFingers(uint8_t finger_pos[]){

    //index
	int I_MCP_flexion = finger_pos[10];
	int I_MCP_abduction = finger_pos[9]+MCP_ABDUCTION_MIN;
    int* I_servo_positions = metacarpalServoCalc(I_MCP_flexion,I_MCP_abduction,true,false);
    IM0_servo.write(I_servo_positions[0]);
    IM1_servo.write(I_servo_positions[1]);
    // Serial.print(I_servo_positions[0]);
	// Serial.print(" ");
	// Serial.println(I_servo_positions[1]);
    delete[] I_servo_positions;
}
