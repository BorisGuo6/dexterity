#include "HallEffectSensors.h"

HallEffectSensors::HallEffectSensors()
{
    this->s0 = D3;
    this->s1 = D4;
    this->s2 = D5;
    this->s3 = D6;
    this->MCP_flexion_min = 0;
    this->MCP_flexion_max = 160;
    this->MCP_abduction_min = -40;
    this->MCP_abduction_max = 40;
    this->peer_mac_addr = {0x3C, 0x84, 0x27, 0xE1, 0xC2, 0x30};
}

HallEffectSensors::HallEffectSensorArr(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, int32_t MCP_flexion_min,
    int32_t MCP_flexion_max, int32_t MCP_abduction_min, int32_t MCP_abduction_max, uint8_t peer_mac_addr[6])
{
    this->s0 = s0;
    this->s1 = s1;
    this->s2 = s2;
    this->s3 = s3;
    this->MCP_flexion_min = MCP_flexion_min;
    this->MCP_flexion_max = MCP_flexion_max;
    this->MCP_abduction_min = MCP_abduction_min;
    this->MCP_abduction_max = MCP_abduction_max;
    this->peer_mac_addr = peer_mac_addr;
}

 void HallEffectSensors::initialize()
  {
    analogReadResolution(12);

    pinMode(D2, OUTPUT);

    pinMode(this->s0, OUTPUT);
    pinMode(this->s1, OUTPUT);
    pinMode(this->s2, OUTPUT);
    pinMode(this->s3, OUTPUT);

    digitalWrite(D2, LOW);

    initializeCalibrationValues();

    glove_ESPNOWsetup(this->peer_mac_addr, 115200);
 }


void HallEffectSensors::updateAngles()
{
    measureAngles();
    calibrate();
    adjustAngles();
}


void HallEffectSensors::measureAngles()
{
    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        digitalWrite(this->s0, i&0b1);
        digitalWrite(this->s1, i>>1&0b1);
        digitalWrite(this->s2, i>>2&0b1);
        digitalWrite(this->s3, i>>3&0b1);

        delay(5); //not sure if this is necessary


        this->analog.update();
        int32_t rawVal = this->analog.getRawValue();
        this->rawVals[i] = rawVal;
     }

    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        int32_t angle = (int32_t)poly(this->rawVals[i],this->polyVals[i][0],this->polyVals[i][1],this->polyVals[i][2]);
        this->proto_angles[i] = angle;
    }
}

void HallEffectSensors::calibrate(){
    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        if(this->proto_angles[i] < this->min_angles[i]){
            this->min_angles[i] = this->proto_angles[i];
        } else if(this->proto_angles[i] > this->max_angles[i]){
            this->max_angles[i] = this->proto_angles[i];
        }
    }
}

void HallEffectSensors::adjustAngles()
{
    //pinkie
    this->angles[0] = adjustMCPAbductionAngle(0);
    this->angles[1] = adjustMCPFlexionAngle(1);
    this->angles[2] = adjustPIPFlexionAngle(2);

    //ring
    this->angles[3] = adjustMCPAbductionAngle(3);
    this->angles[4] = adjustMCPFlexionAngle(4);
    this->angles[5] = adjustPIPFlexionAngle(5);

    //middle
    this->angles[6] = adjustMCPAbductionAngle(6);
    this->angles[7] = adjustMCPFlexionAngle(7);
    this->angles[8] = adjustPIPFlexionAngle(8);

    //index
    this->angles[9] = adjustMCPAbductionAngle(9);
    this->angles[10] = adjustMCPFlexionAngle(10);
    this->angles[11] = adjustPIPFlexionAngle(11);

    //TODO
    //thumb
    this->angles[12] = this->angles[12];
    this->angles[13] = this->angles[13];
    this->angles[14] = this->angles[14];
    this->angles[15] = this->angles[15];
}

int32_t HallEffectSensors::adjustMCPAbductionAngle(int32_t i)
{
    int32_t angle = this->proto_angles[i];
    float32_t max_angle = this->max_angles[i];
    float32_t min_angle = this->min_angles[i];
    int32_t adjusted_angle = (int32_t)((angle - min_angle)/(max_angle-min_angle) * (2*this->MCP_abduction_max));
    return adjusted_angle;
}

int32_t HallEffectSensors::adjustMCPFlexionAngle(int32_t i)
{
    int32_t angle = this->proto_angles[i];
    float32_t max_angle = this->max_angles[i];
    float32_t min_angle = this->min_angles[i];
    int32_t adjusted_angle = (int32_t)((angle - min_angle)/(max_angle-min_angle) * this->MCP_flexion_max);
    return adjusted_angle;
}

//TODO do this properly
int32_t HallEffectSensors::adjustPIPFlexionAngle(int32_t i)
{
    int32_t angle = this->proto_angles[i];
    float32_t max_angle = this->max_angles[i];
    float32_t min_angle = this->min_angles[i];
    int32_t adjusted_angle = (int32_t)((angle - min_angle)/(max_angle-min_angle) * this->MCP_flexion_max);
    return adjusted_angle;
}

void HallEffectSensors::initializeCalibrationValues(){
    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        this->min_angles[i] = 10000;
        this->max_angles[i] = -10000;
    }
}

void HallEffectSensors::printAngles() {
    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        Serial.print(">Joint_");
        Serial.print(i);
        Serial.print(":");
        Serial.println(this->angles[i]);
    }
}

void HallEffectSensors::sendData() {
    uint8_t fpos[SENSOR_COUNT];
    uint8_t wpos[3];
    uint8_t apos[3];
    for(int i = 0; i < SENSOR_COUNT; i++){
        fpos[i] = (uint8_t)this->angles[i];
    }
    for(int i = 0; i < 3; i ++){
        wpos[i] = random(1, 255);
        apos[i] = random(1, 255);
    }

    //position_packet.messages_rec = glove_messages_rcv;

    glove_sendData(fpos, wpos, apos);

    glove_messages_send_attempt += 1;
}

static float32_t poly(double x, double a,double b,double c){
    return a*pow(x,2)+b*x+c;
}