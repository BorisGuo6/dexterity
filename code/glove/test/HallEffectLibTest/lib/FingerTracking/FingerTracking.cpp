#include "FingerTracking.h"
#include "HallEffectSensors.h"

int32_t angles[SENSOR_COUNT];

void fingerTrackingSetup(){
    hallEffectSensorsSetup();
}

void calcFingerAngles()
{
    //TODO measureAngles();
    adjustAngles();
}

void printAngles() {
    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        Serial.print(">Joint_");
        Serial.print(i);
        Serial.print(":");
        Serial.println(angles[i]);
    }
}

void sendData() {
    uint8_t fpos[SENSOR_COUNT];
    uint8_t wpos[3];
    uint8_t apos[3];
    for(int i = 0; i < SENSOR_COUNT; i++){
        fpos[i] = (uint8_t)angles[i];
    }
    for(int i = 0; i < 3; i ++){
        wpos[i] = random(1, 255);
        apos[i] = random(1, 255);
    }

    //position_packet.messages_rec = glove_messages_rcv;

    //glove_sendData(fpos, wpos, apos);

    //glove_messages_send_attempt += 1;
}


int32_t adjustMCPAbductionAngle(int32_t i){
  int32_t angle = proto_angles[i];
  float max_angle = max_angles[i];
  float min_angle = min_angles[i];
  int32_t adjusted_angle = (int32_t)((angle - min_angle)/(max_angle-min_angle) * (2*MCP_ABDUCTION_MAX));
  return adjusted_angle;
}

int32_t adjustMCPFlexionAngle(int32_t i){
  int32_t angle = proto_angles[i];
  float max_angle = max_angles[i];
  float min_angle = min_angles[i];
  int32_t adjusted_angle = (int32_t)((angle - min_angle)/(max_angle-min_angle) * MCP_FLEXION_MAX);
  return adjusted_angle;
}

int32_t adjustPIPFlexionAngle(int32_t i){
  int32_t angle = proto_angles[i];
  float max_angle = max_angles[i];
  float min_angle = min_angles[i];
  int32_t adjusted_angle = (int32_t)((angle - min_angle)/(max_angle-min_angle) * PIP_FLEXION_MAX);
  return adjusted_angle;
}

int32_t adjustThumbCMCAbductionAngle(int32_t i){
  int32_t angle = proto_angles[i];
  float max_angle = max_angles[i];
  float min_angle = min_angles[i];
  int32_t adjusted_angle = (int32_t)((angle - min_angle)/(max_angle-min_angle) * (2*THUMB_CMC_ABDUCTION_MAX));
  return adjusted_angle;
}

int32_t adjustThumbCMCFlexionAngle(int32_t i){
  int32_t angle = proto_angles[i];
  float max_angle = max_angles[i];
  float min_angle = min_angles[i];
  int32_t adjusted_angle = (int32_t)((angle - min_angle)/(max_angle-min_angle) * THUMB_CMC_FLEXION_MAX);
  return adjusted_angle;
}

int32_t adjustThumbPIPFlexionAngle(int32_t i){
  int32_t angle = proto_angles[i];
  float max_angle = max_angles[i];
  float min_angle = min_angles[i];
  int32_t adjusted_angle = (int32_t)((angle - min_angle)/(max_angle-min_angle) * THUMB_PIP_FLEXION_MAX);
  return adjusted_angle;
}

void adjustAngles()
{
    //pinkie
    angles[0] = adjustMCPAbductionAngle(0);
    angles[1] = adjustMCPFlexionAngle(1);
    angles[2] = adjustPIPFlexionAngle(2);

    //ring
    angles[3] = adjustMCPAbductionAngle(3);
    angles[4] = adjustMCPFlexionAngle(4);
    angles[5] = adjustPIPFlexionAngle(5);

    //middle
    angles[6] = adjustMCPAbductionAngle(6);
    angles[7] = adjustMCPFlexionAngle(7);
    angles[8] = adjustPIPFlexionAngle(8);

    //index
    angles[9] = adjustMCPAbductionAngle(9);
    angles[10] = adjustMCPFlexionAngle(10);
    angles[11] = adjustPIPFlexionAngle(11);

    //TODO
    //thumb
    angles[12] = adjustThumbCMCFlexionAngle(12);
    angles[13] = adjustThumbCMCAbductionAngle(13);
    angles[14] = adjustThumbPIPFlexionAngle(14);
    angles[15] = proto_angles[15]; //not using this data currently
}

