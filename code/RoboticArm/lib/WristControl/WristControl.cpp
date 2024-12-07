#include "WristControl.h"

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
    uint16_t pos3 = 511;
    uint16_t pos4 = 511;
    if(200 > pos3){
        pos3 = 200;
    }
    if(pos3 > 800){
        pos3 = 800;
    }
    if(200 > pos4){
        pos4 = 200;
    }
    if(pos4 > 800){
        pos4 = 800;
    }
    SERVO.WritePos(SERVO_ID_3, pos3%1000, 100, 20);
    SERVO.WritePos(SERVO_ID_4, pos4%1000, 100, 20);
    return;
}