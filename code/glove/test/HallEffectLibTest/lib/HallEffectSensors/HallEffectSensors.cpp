#include "HallEffectSensors.h"

ResponsiveAnalogRead analog(A1, true);

int32_t rawVals[SENSOR_COUNT];
int32_t proto_angles[SENSOR_COUNT];
int32_t angles[SENSOR_COUNT];
int32_t min_angles[SENSOR_COUNT];
int32_t max_angles[SENSOR_COUNT];

float polyVals[16][3] = {
    {-0.000050156739812,0.308087774294671,-325.54858934169279}, //pinkie 0
    {-0.000204869267408,1.180238586110067,-1522.071698458919325}, //pinkie 1
    {0.00009027900176,-0.57849114376526,925.953643298021097}, //pinkie 2
    {-0.000053897180763,0.344485903814262,-417.201492537313433}, //ring 3
    {-0.000406773372404,2.026886777162145,-2364.087785492362531}, //ring 4
    {0.000020669692875,-0.186416998438648,403.233727023548938}, //ring 5
    {-0.000041474654378,0.275529953917051,-295.161290322580645}, //middle 6
    {-0.000155663598998,0.846081469596033,-994.321241823930591}, //middle 7
    {0.000170233984067,-1.128460118194487,1768.951835332448657}, //middle 8
    {-0.000005288207298,0.121258593336859,-147.245901639344262}, //pointer 9
    {-0.000135942468348,0.817456387325188,-1033.093236601650843}, //pointer 10
    {0.000101643291297,-0.646716069346575,1031.971761445997989}, //pointer 11
    {-0.000087481431887,0.549306011516565,-709.440158534950912}, //thumb 12
    {0.000043188683603,-0.288631646308703,+518.26001946236131}, //thumb 13
    {0.000081944493116,-0.48715545187493,753.215310445897971}, //thumb 14
    {0.000029299702805,-0.235958663536102,473.892439373476074}  //thumb 15
};

void hallEffectSensorsSetup(){
    analogReadResolution(12);

    //pinMode(D2, OUTPUT);

    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);

    //digitalWrite(D2, LOW);

    initializeCalibrationValues();

    for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
        angles[i] = 0;
    }
}

float poly(double x, double a,double b,double c){
    return a*pow(x,2)+b*x+c;
}

void updateAngles()
{
    measureAngles();
    calibrate();
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

void initializeCalibrationValues(){
    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        min_angles[i] = 10000;
        max_angles[i] = -10000;
    }
}

void calibrate(){
    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        if(proto_angles[i] < min_angles[i]){
            min_angles[i] = proto_angles[i];
        } else if(proto_angles[i] > max_angles[i]){
            max_angles[i] = proto_angles[i];
        }
    }
}

void measureAngles()
{
    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        digitalWrite(S0, i & 0b1);
        digitalWrite(S1, (i>>1) & 0b1);
        digitalWrite(S2, (i>>2) & 0b1);
        digitalWrite(S3, (i>>3) & 0b1);

        delay(5); //not sure if this is necessary


        analog.update();
        int32_t rawVal = analog.getRawValue();
        rawVals[i] = rawVal;
     }

    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        int32_t angle = (int32_t)poly(rawVals[i],polyVals[i][0],polyVals[i][1],polyVals[i][2]);
        proto_angles[i] = angle;
    }
    //jank solution to having the angles for the thumb backwards
    //TODO remove with glove v2
    proto_angles[12] = 150-proto_angles[12];
    proto_angles[13] = 150-proto_angles[12];
    proto_angles[14] = 150-proto_angles[14];
    proto_angles[15] = 150-proto_angles[15];
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

