#include "FingerControl.h"

void fingerControlSetup(){
    emaxServoControlSetup();
}

int* metacarpalServoCalc(int MCP_flexion, int MCP_abduction, bool flip_abduction){
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

    Serial.printf("%d %d\n",servo0_offset,servo1_offset);


    // //constrain to limits
	// int servo0_pos = constrain(MCP_flexion+servo0_offset,MCP_FLEXION_MIN,MCP_FLEXION_MAX);
	// int servo1_pos = constrain(MCP_flexion+servo1_offset,MCP_FLEXION_MIN,MCP_FLEXION_MAX);
    int servo0_pos = MCP_flexion+servo0_offset;
	int servo1_pos = MCP_flexion+servo1_offset;

    //allocate memeory for servo position array
    int* servo_positions = new int[2];
    servo_positions[0] = servo0_pos;
    servo_positions[1] = servo1_pos;

    return servo_positions;
}

int tipServoCalc(int flexion){
    int servo_pos = flexion;
    servo_pos=constrain(servo_pos,MCP_FLEXION_MIN,100);
    return servo_pos;
}

void controlFingers(uint8_t finger_pos[]){

    for(uint8_t i = 0; i < 4; i++){
	    controlFinger(i, finger_pos);
    }

    // // int I_tip_flexion = finger_pos[11];
    // // int I_tip_servo_pos = tipServoCalc(I_tip_flexion,false);
    // // IT_servo.write(I_tip_servo_pos);

    // //middle
	// int M_MCP_flexion = finger_pos[7];
	// int M_MCP_abduction = finger_pos[6]+MCP_ABDUCTION_MIN;
    // int* M_servo_positions = metacarpalServoCalc(M_MCP_flexion,M_MCP_abduction,true);
    // // MM0_servo.write(M_servo_positions[0]);
    // // MM1_servo.write(M_servo_positions[1]);
    // // Serial.print(M_servo_positions[0]);
	// // Serial.print(" ");
	// // Serial.println(M_servo_positions[1]);
    // delete[] M_servo_positions;

    // //ring
	// int R_MCP_flexion = finger_pos[4];
	// int R_MCP_abduction = finger_pos[3]+MCP_ABDUCTION_MIN;
    // int* R_servo_positions = metacarpalServoCalc(R_MCP_flexion,R_MCP_abduction,false);
    // // RM0_servo.write(R_servo_positions[0]);
    // // RM1_servo.write(R_servo_positions[1]);
    // delete[] R_servo_positions;

    // //pinkie
	// int P_MCP_flexion = finger_pos[1];
	// int P_MCP_abduction = finger_pos[0]+MCP_ABDUCTION_MIN;
    // int* P_servo_positions = metacarpalServoCalc(P_MCP_flexion,P_MCP_abduction,false);
    // // PM0_servo.write(P_servo_positions[0]);
    // // PM1_servo.write(P_servo_positions[1]);
    // delete[] P_servo_positions;
}

void controlFinger(uint8_t finger_num, uint8_t finger_pos[]){

    int MCP_flexion, MCP_abduction, PIP_flexion;
    uint8_t M0_servo_num, M1_servo_num, T_servo_num;
    bool flip_abduction = false;
    switch(finger_num){
        case 0: // Index Finger
            MCP_flexion = finger_pos[10];
            MCP_abduction = finger_pos[9] + MCP_ABDUCTION_MIN;
            flip_abduction = true; // Example logic
            M0_servo_num = 13;
            M1_servo_num = 12;
            T_servo_num = 7;
            break;

        case 1: // Middle Finger
            MCP_flexion = finger_pos[7];
            MCP_abduction = finger_pos[6] + MCP_ABDUCTION_MIN;
            flip_abduction = true;
            M0_servo_num = 15;
            M1_servo_num = 14;
            T_servo_num = 6;
            break;

        case 2: // Ring Finger
            MCP_flexion = finger_pos[4];
            MCP_abduction = finger_pos[3] + MCP_ABDUCTION_MIN;
            flip_abduction = false;
            M0_servo_num = 9;
            M1_servo_num = 8;
            T_servo_num = 5;
            break;

        case 3: // Pinkie Finger
            MCP_flexion = finger_pos[1];
            MCP_abduction = finger_pos[0] + MCP_ABDUCTION_MIN;
            flip_abduction = false;
            M0_servo_num = 10;
            M1_servo_num = 11;
            T_servo_num = 4;
            break;

        default: // Unknown Finger
            Serial.println("Error: Invalid finger number!");
            return; // Exit the function for invalid finger numbers
    }
    int* servo_positions = metacarpalServoCalc(MCP_flexion,MCP_abduction,flip_abduction);
    controlEmaxServo(M0_servo_num, servo_positions[0]);
    controlEmaxServo(M1_servo_num, servo_positions[1]);
    Serial.printf("%d %d | %d %d\n",MCP_flexion,MCP_abduction,servo_positions[0],servo_positions[1]);

    delete[] servo_positions;

    int tip_pos = tipServoCalc(PIP_flexion);
    controlEmaxServo(T_servo_num, servo_positions[0]);
}


