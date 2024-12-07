#include "PressureSensors.h"

// IMPORTANT: IMPLEMENT PRESSURE SENSOR TO ARM CONTROL IN FUTURE
uint8_t hset[5];
uint32_t forces[5];
uint8_t iters;

void setupPressureSensors(){
    iters = 0;
    return;
}

// IMPORTANT: IMPLEMENT PRESSURE SENSOR READINGS
void pollPressureSensors(){
    forces[0] = analogRead(FI_TEST_PIN);
    forces[1] = 0;
    forces[2] = 0;
    forces[3] = 0;
    forces[4] = 0;
}

void sendPressureData(){
    pollPressureSensors();
    for(int j=0; j<5; j++){
        if(forces[j] < 700) hset[j] = 0;
        else if(forces[j] < 1700) hset[j] = 1;
        else if(forces[j] < 2700) hset[j] = 2;
        else if(forces[j] < 3400) hset[j] = 3;
        else if(forces[j] < 3800) hset[j] = 4;
        else hset[j] = 5;
    }
    if(ENABLE_PRESSURE_PRINT && iters%10 == 0){
        Serial.print("ADC Values: ");
        for(int j=0; j<5; j++){
            Serial.print(forces[j]);
            Serial.print("\t");
        }
        Serial.println();
    }
    iters += 1;
    arm_sendData(hset[0], hset[1], hset[2], hset[3], hset[4]);
    return;
}