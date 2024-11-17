#include "FingerControl.h"

Servo IM0_servo;
Servo IM1_servo;
Servo IT_servo;

Servo MM0_servo;
Servo MM1_servo;
Servo MT_servo;

Servo RM0_servo;
Servo RM1_servo;
Servo RT_servo;

Servo PM0_servo;
Servo PM1_servo;
Servo PT_servo;

ESP32PWM pwm;

void fingerControlSetup(){
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    IM0_servo.setPeriodHertz(50); 
    IM1_servo.setPeriodHertz(50); 
    IT_servo.setPeriodHertz(50); 

    MM0_servo.setPeriodHertz(50); 
    MM1_servo.setPeriodHertz(50);
    //MT_servo.setPeriodHertz(50); 

    RM0_servo.setPeriodHertz(50); 
    RM1_servo.setPeriodHertz(50);
    //RT_servo.setPeriodHertz(50); 

    PM0_servo.setPeriodHertz(50); 
    PM1_servo.setPeriodHertz(50);
    //PT_servo.setPeriodHertz(50); 

    IM0_servo.attach(IM0_SERVO_PIN, MIN_US, MAX_US);
    IM1_servo.attach(IM1_SERVO_PIN, MIN_US, MAX_US);
    //IT_servo.attach(IT_SERVO_PIN, MIN_US, MAX_US);

    MM0_servo.attach(MM0_SERVO_PIN, MIN_US, MAX_US);
    MM1_servo.attach(MM1_SERVO_PIN, MIN_US, MAX_US);
    //MT_servo.attach(MT_SERVO_PIN, MIN_US, MAX_US);

    RM0_servo.attach(RM0_SERVO_PIN, MIN_US, MAX_US);
    RM1_servo.attach(RM1_SERVO_PIN, MIN_US, MAX_US);
    //RT_servo.attach(RT_SERVO_PIN, MIN_US, MAX_US);

    PM0_servo.attach(PM0_SERVO_PIN, MIN_US, MAX_US);
    PM1_servo.attach(PM1_SERVO_PIN, MIN_US, MAX_US);
    //PT_servo.attach(PT_SERVO_PIN, MIN_US, MAX_US);
}

int* metacarpalServoCalc(int MCP_flexion, int MCP_abduction, bool flip_abduction, bool flip_M0_servo, bool flip_M1_servo){
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

    //option to flip abduction direction
    if(flip_abduction){
        int temp = servo0_offset;
        servo0_offset = servo1_offset;
        servo1_offset = temp;
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

int tipServoCalc(int flexion, bool flip_servo){
    int servo_pos = flexion;
    servo_pos=constrain(servo_pos,MCP_FLEXION_MIN,100);
    if(flip_servo){
        servo_pos=180-servo_pos;
    }
    return servo_pos;
}

void controlFingers(uint8_t finger_pos[]){

    //index
	int I_MCP_flexion = finger_pos[10];
	int I_MCP_abduction = finger_pos[9]+MCP_ABDUCTION_MIN;
    int* I_servo_positions = metacarpalServoCalc(I_MCP_flexion,I_MCP_abduction,true,true,false);
    IM0_servo.write(I_servo_positions[0]);
    IM1_servo.write(I_servo_positions[1]);
    delete[] I_servo_positions;

    // int I_tip_flexion = finger_pos[11];
    // int I_tip_servo_pos = tipServoCalc(I_tip_flexion,false);
    // IT_servo.write(I_tip_servo_pos);

    //middle
	int M_MCP_flexion = finger_pos[7];
	int M_MCP_abduction = finger_pos[6]+MCP_ABDUCTION_MIN;
    int* M_servo_positions = metacarpalServoCalc(M_MCP_flexion,M_MCP_abduction,true,false,false);
    MM0_servo.write(M_servo_positions[0]);
    MM1_servo.write(M_servo_positions[1]);
    // Serial.print(M_servo_positions[0]);
	// Serial.print(" ");
	// Serial.println(M_servo_positions[1]);
    delete[] M_servo_positions;

    //ring
	int R_MCP_flexion = finger_pos[4];
	int R_MCP_abduction = finger_pos[3]+MCP_ABDUCTION_MIN;
    int* R_servo_positions = metacarpalServoCalc(R_MCP_flexion,R_MCP_abduction,false,true,true);
    RM0_servo.write(R_servo_positions[0]);
    RM1_servo.write(R_servo_positions[1]);
    delete[] R_servo_positions;

    //pinkie
	int P_MCP_flexion = finger_pos[1];
	int P_MCP_abduction = finger_pos[0]+MCP_ABDUCTION_MIN;
    int* P_servo_positions = metacarpalServoCalc(P_MCP_flexion,P_MCP_abduction,false,false,true);
    PM0_servo.write(P_servo_positions[0]);
    PM1_servo.write(P_servo_positions[1]);
    delete[] P_servo_positions;
}


