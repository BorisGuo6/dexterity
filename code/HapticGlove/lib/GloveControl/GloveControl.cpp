#include "GloveControl.h"
#include "IMU.hpp"

float wpos[3];

void gloveControlSetup(){
  // initialize IMUs, Hall-Effect Sensors, etc
  wpos[0] = 0;
  wpos[1] = 0;
  wpos[2] = 0;
  // NOTE: IMUs init and cal need to be ran prior to ISRs
  return;
}

void sendPositionData(){
  // get IMU euler angles
  if(IMUS_CONNECTED){
    euler_t ypr = getAngleDifference();
    // format wrist position data
    wpos[0] = ypr.yaw;
    wpos[1] = ypr.pitch;
    wpos[2] = ypr.roll;
  }

  // format data
  uint8_t fpos[16];

  if(ENABLE_SENSORS_PRINT && IMUS_CONNECTED){
    Serial.print(wpos[0]); Serial.print(" ");
    Serial.print(wpos[1]); Serial.print(" ");
    Serial.print(wpos[2]); Serial.println();
  }

  // format arm position data
  uint8_t apos[3];
  
  // fill with random data (REMOVE LATER)
  for(int j=0; j<16; j++){
    fpos[j] = random(1, 255);
  }
  for(int j=0; j<3; j++){
    //wpos[j] = random(1, 255);
    apos[j] = random(1, 255);
  }

  // send data
  glove_sendData(fpos, wpos, apos);
}