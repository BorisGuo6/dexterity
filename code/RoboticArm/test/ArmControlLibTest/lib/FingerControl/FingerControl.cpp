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


    // //constrain to limits
	int servo0_pos = constrain(MCP_flexion+servo0_offset,MCP_FLEXION_MIN,MCP_FLEXION_MAX);
	int servo1_pos = constrain(MCP_flexion+servo1_offset,MCP_FLEXION_MIN,MCP_FLEXION_MAX);
    // int servo0_pos = MCP_flexion+servo0_offset;
	// int servo1_pos = MCP_flexion+servo1_offset;

    //allocate memeory for servo position array
    int* servo_positions = new int[2];
    servo_positions[0] = servo0_pos;
    servo_positions[1] = servo1_pos;

    return servo_positions;
}

int tipServoCalc(int flexion){
    int servo_pos = flexion;
    servo_pos=constrain(servo_pos,PIP_FLEXION_MIN,PIP_FLEXION_MAX);
    return servo_pos;
}

int* thumbCMCServoCalc(int CMC_flexion, int CMC_abduction, bool flip_abduction){
    //servo offset for abduction
	int servo0_offset = 0;
	int servo1_offset = 0;

    //calculate how much the servos should offset from each other for abduction
	if(CMC_abduction > 0){ //right
		CMC_abduction = max(CMC_abduction,THUMB_CMC_ABDUCTION_MIN);
		servo1_offset=-CMC_abduction;
	}else{ //left
		CMC_abduction = min(CMC_abduction,THUMB_CMC_ABDUCTION_MAX);
		servo0_offset=CMC_abduction;
	}

    //option to flip abduction direction
    if(flip_abduction){
        int temp = servo0_offset;
        servo0_offset = servo1_offset;
        servo1_offset = temp;
    }


    // //constrain to limits
	int servo0_pos = constrain(CMC_flexion+servo0_offset,THUMB_CMC_FLEXION_MIN,THUMB_CMC_FLEXION_MAX);
	int servo1_pos = constrain(CMC_flexion+servo1_offset,THUMB_CMC_FLEXION_MIN,THUMB_CMC_FLEXION_MAX);

    //allocate memeory for servo position array
    int* servo_positions = new int[2];
    servo_positions[0] = servo0_pos;
    servo_positions[1] = servo1_pos;

    return servo_positions;
}

int thumbTipServoCalc(int flexion){
    int servo_pos = flexion;
    servo_pos=constrain(servo_pos,THUMB_PIP_FLEXION_MIN,THUMB_PIP_FLEXION_MAX);
    return servo_pos;
}

int thumbRotationServoCalc(int rotation){
    int servo_pos = rotation*2;
    servo_pos=constrain(servo_pos,THUMB_ROTATION_MIN,THUMB_ROTATION_MAX);
    return servo_pos;
}

void controlFingers(uint8_t finger_pos[]){

    for(uint8_t i = 0; i < 4; i++){
	    controlFinger(i, finger_pos);
    }

    controlThumb(finger_pos);

    //reset counter
    servo_position_num=0;

}

void controlFinger(uint8_t finger_num, uint8_t finger_pos[]){

    int MCP_flexion, MCP_abduction, PIP_flexion;
    uint8_t M0_servo_num, M1_servo_num, T_servo_num;
    bool flip_abduction = false;
    switch(finger_num){
        case 3: // Index Finger
            PIP_flexion = finger_pos[11];
            MCP_flexion = finger_pos[10];
            MCP_abduction = finger_pos[9] + MCP_ABDUCTION_MIN;
            flip_abduction = true; // Example logic
            M0_servo_num = IM0_SERVO_PIN;
            M1_servo_num = IM1_SERVO_PIN;
            T_servo_num = IT_SERVO_PIN;
            break;

        case 2: // Middle Finger
            PIP_flexion = finger_pos[8];
            MCP_flexion = finger_pos[7];
            MCP_abduction = finger_pos[6] + MCP_ABDUCTION_MIN;
            flip_abduction = true;
            M0_servo_num = MM0_SERVO_PIN;
            M1_servo_num = MM1_SERVO_PIN;
            T_servo_num = MT_SERVO_PIN;
            break;

        case 1: // Ring Finger
            PIP_flexion = finger_pos[5];
            MCP_flexion = finger_pos[4];
            MCP_abduction = finger_pos[3] + MCP_ABDUCTION_MIN;
            flip_abduction = false;
            M0_servo_num = RM0_SERVO_PIN;
            M1_servo_num = RM1_SERVO_PIN;
            T_servo_num = RT_SERVO_PIN;
            break;

        case 0: // Pinkie Finger
            PIP_flexion = finger_pos[2];
            MCP_flexion = finger_pos[1];
            MCP_abduction = finger_pos[0] + MCP_ABDUCTION_MIN;
            flip_abduction = false;
            M0_servo_num = PM0_SERVO_PIN;
            M1_servo_num = PM1_SERVO_PIN;
            T_servo_num = PT_SERVO_PIN;
            break;

        default: // Unknown Finger
            Serial.println("Error: Invalid finger number!");
            return; // Exit the function for invalid finger numbers
    }
    int* servo_positions = metacarpalServoCalc(MCP_flexion,MCP_abduction,flip_abduction);
    controlEmaxServo(M0_servo_num, servo_positions[0]);
    controlEmaxServo(M1_servo_num, servo_positions[1]);


    delete[] servo_positions;

    int tip_pos = tipServoCalc(PIP_flexion);
    controlEmaxServo(T_servo_num, tip_pos);
}

void controlThumb(uint8_t finger_pos[]){

    int CMC_flexion = finger_pos[12];
    int CMC_abduction = finger_pos[13] + THUMB_CMC_ABDUCTION_MIN;
    int PIP_flexion = finger_pos[14];

    int* servo_positions = thumbCMCServoCalc(CMC_flexion,CMC_abduction,true);
    controlEmaxServo(TM0_SERVO_PIN, servo_positions[0]);
    controlEmaxServo(TM1_SERVO_PIN, servo_positions[1]);

    int tip_pos = thumbTipServoCalc(PIP_flexion);
    controlEmaxServo(TT_SERVO_PIN, tip_pos);

    int rotation_pos = thumbRotationServoCalc(finger_pos[13]);
    controlEmaxServo(TROT_SERVO_PIN, rotation_pos);

    delete[] servo_positions;

}

void moveServoThroughSerial(){
    while(1){
        Serial.println("Enter servo: ");
        while(Serial.available() == 0){};
        int servo = Serial.readString().toInt();
        Serial.println("Enter position: ");
        while(Serial.available() == 0){};
        int pos = Serial.readString().toInt();
        if (pos >= 0 && pos <= 255){
            controlEmaxServo(servo, pos);
            Serial.print("Turned ");
            Serial.print(servo);
            Serial.print(" to: ");
            Serial.println(pos);
        }
        else {
            Serial.println("Invalid postition!");
        }
    }
}

void printEmaxServoPositions(){
    servo_position_num=0;
    Serial.printf(">PM0:%d \n>PM1:%d \n>PT:%d \n",all_servo_positions[servo_position_num++], all_servo_positions[servo_position_num++], all_servo_positions[servo_position_num++]);
    Serial.printf(">RM0:%d \n>RM1:%d \n>RT:%d \n",all_servo_positions[servo_position_num++], all_servo_positions[servo_position_num++], all_servo_positions[servo_position_num++]);
    Serial.printf(">MM0:%d \n>MM1:%d \n>MT:%d \n",all_servo_positions[servo_position_num++], all_servo_positions[servo_position_num++], all_servo_positions[servo_position_num++]);
    Serial.printf(">IM0:%d \n>IM1:%d \n>IT:%d \n",all_servo_positions[servo_position_num++], all_servo_positions[servo_position_num++], all_servo_positions[servo_position_num++]);

    Serial.printf(">TM0:%d \n>TM1:%d \n>TT:%d \n>TR:%d \n",all_servo_positions[servo_position_num++], all_servo_positions[servo_position_num++], all_servo_positions[servo_position_num++], all_servo_positions[servo_position_num++]);

    servo_position_num=0;
}



