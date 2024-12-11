#include "WristControl.h"

#define SERVO3_CENTER_POSITION 511
#define SERVO4_CENTER_POSITION 511
#define BITS_PER_DEGREE ((float) (1024.0 / 180.0))
#define SERVO3_LOWER_LIMIT 20
#define SERVO3_UPPER_LIMIT 1000
#define SERVO4_LOWER_LIMIT 20
#define SERVO4_UPPER_LIMIT 1000
#define WRIST_SERVO_SPEED  500

SCServo SERVO;

void wristControlSetup(){
    Serial2.begin(1000000, SERIAL_8N1, RX_PIN_2, TX_PIN_2); // Initialize Serial2 for servo communication
    SERVO.pSerial = &Serial2; // Assign Serial to the SCServo instance
    return;
}

void controlWrist(float* wrist_pos){
    if(ENABLE_CONTROL_PRINT || ENABLE_WRIST_PRINT){
        Serial.print("wpos - ");
        Serial.print(wrist_pos[0]);
        Serial.print(" ");
        Serial.print(wrist_pos[1]);
        Serial.print(" ");
        Serial.println(wrist_pos[2]);
    }
   // convert euler angles to servo commands
    float yaw =  -1 * wrist_pos[0];
    float pitch = wrist_pos[1];
    float servo3_angle = pitch + yaw;
    float servo4_angle = yaw - pitch;
    uint16_t pos3_diff = (uint16_t) (BITS_PER_DEGREE * servo3_angle);
    uint16_t pos4_diff = (uint16_t) (BITS_PER_DEGREE * servo4_angle);
    uint16_t pos3 = (uint16_t) (pos3_diff + SERVO3_CENTER_POSITION);
    uint16_t pos4 = (uint16_t) (pos4_diff + SERVO3_CENTER_POSITION);
    
    // set limits for servo 3
    if(SERVO3_LOWER_LIMIT > pos3){
        pos3 = SERVO3_LOWER_LIMIT;
    }
    if(pos3 > SERVO3_UPPER_LIMIT){
        pos3 = SERVO3_UPPER_LIMIT;
    }

    // set limits for servo 4
    if(SERVO4_LOWER_LIMIT > pos4){
        pos4 = SERVO4_LOWER_LIMIT;
    }
    if(pos4 > SERVO4_UPPER_LIMIT){
        pos4 = SERVO4_UPPER_LIMIT;
    }

    // send position to servos
    SERVO.WritePos(SERVO_ID_3, pos3%1024, 100, WRIST_SERVO_SPEED);
    SERVO.WritePos(SERVO_ID_4, pos4%1024, 100, WRIST_SERVO_SPEED);
    return;
}