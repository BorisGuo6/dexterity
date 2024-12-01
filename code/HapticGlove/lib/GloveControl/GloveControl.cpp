#include "GloveControl.h"
#include "IMU.hpp"

void gloveControlSetup(){
  // initialize IMUs, Hall-Effect Sensors, etc

  // NOTE: IMUs init and cal need to be ran prior to ISRs
  return;
}

void sendPositionData(){
  // get IMU euler angles
  euler_t ypr = getAngleDifference();

  // format data
  uint8_t fpos[16];

  // format wrist position data
  float wpos[3];
  wpos[0] = ypr.yaw;
  wpos[1] = ypr.pitch;
  wpos[2] = ypr.roll;

  Serial.print(wpos[0]); Serial.print(" ");
  Serial.print(wpos[1]); Serial.print(" ");
  Serial.print(wpos[2]); Serial.println();

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