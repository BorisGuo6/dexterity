#include "PressureSensors.h"

// IMPORTANT: IMPLEMENT PRESSURE SENSOR TO ARM CONTROL IN FUTURE
uint8_t fi;
uint8_t fm;
uint8_t fr;
uint8_t fp;
uint8_t ft;
uint8_t k;
unsigned long time1_elapsed;

void setupPressureSensors(){
    k = 0;
}

// IMPORTANT: IMPLEMENT PRESSURE SENSOR READINGS
void pollPressureSensors(){
    fi = 0;
    fm = 0;
    fr = 0;
    fp = 0;
    ft = 0;
    return;
}

void sendPressureData(){
    if(ENABLE_PRESSURE_PRINT){
        Serial.print("Sending pressure data from core ");
        Serial.println(xPortGetCoreID());
    }
    pollPressureSensors();
    arm_sendData(fi, fm, fr, fp, ft);
    
    k++;
    if(k%200 == 0 && TRACK_ISR_1){
    Serial.print("\ncallback 1 run at "); Serial.print((200*1000000)/(micros() - time1_elapsed)); Serial.println(" Hz\n");
    k = 0;
    time1_elapsed = micros();
  }
}