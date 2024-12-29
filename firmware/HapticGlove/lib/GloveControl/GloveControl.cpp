#include "GloveControl.h"
#include "IMU.hpp"

float wpos[3];
uint8_t fpos[16];
uint8_t apos[3];
uint8_t i;
unsigned long time1_elapsed;

void gloveControlSetup(){
  // initialize IMUs, Hall-Effect Sensors, etc
  i = 0;
  for(int j=0; j<3; j++){
    apos[j] = 0;
    wpos[j] = 0;
  }
  for(int j=0; j<SENSOR_COUNT; j++){
    fpos[j] = 0;
  }
  if(TRACK_ISR_1) time1_elapsed = micros();
  // NOTE: IMUs init and cal need to be ran prior to ISRs
  return;
}

void sendPositionData(){
  // get IMU euler angles
  // if(button_pressed){
  //   Serial.println("button pressed");
  //   calibrateHallEffectSensors();
  //   vTaskDelay(1000 / portTICK_PERIOD_MS);
  //   button_pressed = false;
  // }

  if(IMUS_CONNECTED){
    euler_t ypr = getAngleDifference();
    // format wrist position data
    wpos[0] = ypr.yaw;
    wpos[1] = ypr.pitch;
    wpos[2] = ypr.roll;
  }

  if(ENABLE_IMU_SENSORS_PRINT && IMUS_CONNECTED){
    Serial.print(wpos[0]); Serial.print(" ");
    Serial.print(wpos[1]); Serial.print(" ");
    Serial.print(wpos[2]); Serial.println();
  }
  
  calcFingerAngles();
  for(int j=0; j<SENSOR_COUNT; j++){
    fpos[j] = (uint8_t)angles[j];
  }

  if(ENABLE_SENSORS_PRINT){
    Serial.print("fpos - ");
    for(int j=0; j<SENSOR_COUNT; j++){
      Serial.print(fpos[j]); Serial.print(" ");
    }
    Serial.println();
  }
  if(ENABLE_SENSORS_PRINT && IMUS_CONNECTED){
    Serial.print("wpos - ");
    Serial.print(wpos[0]); Serial.print(" ");
    Serial.print(wpos[1]); Serial.print(" ");
    Serial.print(wpos[2]); Serial.println();
    Serial.println("\n");
  }

  // send data
  glove_sendData(fpos, wpos, apos);

  i++;
  if(i%200 == 0 && TRACK_ISR_1){
    Serial.print("\ncallback 1 run at "); Serial.print((200*1000000)/(micros() - time1_elapsed)); Serial.println(" Hz\n");
    i = 0;
    time1_elapsed = micros();
  }
}