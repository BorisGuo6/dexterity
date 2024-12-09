#include "PressureSensors.h"

// IMPORTANT: IMPLEMENT PRESSURE SENSOR TO ARM CONTROL IN FUTURE
uint8_t force_data[5];
uint8_t force_settings[5];
uint8_t k;
uint8_t iters;
unsigned long time1_elapsed;

void setupPressureSensors(){
    k = 0;
    iters = 0;
}

// IMPORTANT: IMPLEMENT PRESSURE SENSOR READINGS
void pollPressureSensors(){
    if(!PRESSURE_SENSORS_ENABLED){
        if(k<33){
            force_settings[1] = 1; force_settings[2] = 0; force_settings[3] = 0; force_settings[4] = 0; force_settings[0] = 5;
        }
        else if(k<66){
            force_settings[1] = 3; force_settings[2] = 1; force_settings[3] = 0; force_settings[4] = 0; force_settings[0] = 0;
        }
        else if(k<100){
            force_settings[1] = 5; force_settings[2] = 3; force_settings[3] = 1; force_settings[4] = 0; force_settings[0] = 0;
        }
        else if(k<133){
            force_settings[1] = 0; force_settings[2] = 5; force_settings[3] = 3; force_settings[4] = 1; force_settings[0] = 0;
        }
        else if(k<166){
            force_settings[1] = 0; force_settings[2] = 0; force_settings[3] = 5; force_settings[4] = 3; force_settings[0] = 1;
        }
        else{
            force_settings[1] = 0; force_settings[2] = 0; force_settings[3] = 0; force_settings[4] = 5; force_settings[0] = 3;
        }
    }
    else{
        force_data[1] = analogRead(PRESSURE_SENSOR_1); 
        force_data[2] = analogRead(PRESSURE_SENSOR_2); 
        force_data[3] = analogRead(PRESSURE_SENSOR_3); 
        force_data[4] = analogRead(PRESSURE_SENSOR_4); 
        force_data[0] = analogRead(PRESSURE_SENSOR_0);
    }
}

void sendPressureData(){
    pollPressureSensors();
    if(PRESSURE_SENSORS_ENABLED){
        for(int j=0; j<5; j++){
            if(force_data[j] < 700) force_settings[j] = 0;
            else if(force_data[j] < 1700) force_settings[j] = 1;
            else if(force_data[j] < 2700) force_settings[j] = 2;
            else if(force_data[j] < 3400) force_settings[j] = 3;
            else if(force_data[j] < 3800) force_settings[j] = 4;
            else force_settings[j] = 5;
        }
    }
    if(ENABLE_PRESSURE_PRINT && iters%10 == 0){
        Serial.print("ADC Values: ");
        for(int j=0; j<5; j++){
            Serial.print(force_data[j]);
            Serial.print("\t");
        }
        Serial.println();
    }
    iters += 1;
    arm_sendData(force_settings[0], force_settings[1], force_settings[2], force_settings[3], force_settings[4]);
    k++;
    if(k%200 == 0 && TRACK_ISR_1){
    Serial.print("\ncallback 1 run at "); Serial.print((200*1000000)/(micros() - time1_elapsed)); Serial.println(" Hz\n");
    k = 0;
    time1_elapsed = micros();
  }
}