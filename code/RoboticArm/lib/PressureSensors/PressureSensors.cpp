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
    if(k<33){
        fi = 1; fm = 0; fr = 0; fp = 0; ft = 5;
    }
    else if(k<66){
        fi = 3; fm = 1; fr = 0; fp = 0; ft = 0;
    }
    else if(k<100){
        fi = 5; fm = 3; fr = 1; fp = 0; ft = 0;
    }
    else if(k<133){
        fi = 0; fm = 5; fr = 3; fp = 1; ft = 0;
    }
    else if(k<166){
        fi = 0; fm = 0; fr = 5; fp = 3; ft = 1;
    }
    else{
        fi = 0; fm = 0; fr = 0; fp = 5; ft = 3;
    }
}

void sendPressureData(){
    if(ENABLE_PRESSURE_PRINT){
        Serial.print("Sending pressure data from core ");
        Serial.println(xPortGetCoreID());
    }
    pollPressureSensors();
    arm_sendData(ft, fi, fm, fr, fp);
    
    k++;
    if(k%200 == 0 && TRACK_ISR_1){
    Serial.print("\ncallback 1 run at "); Serial.print((200*1000000)/(micros() - time1_elapsed)); Serial.println(" Hz\n");
    k = 0;
    time1_elapsed = micros();
  }
}