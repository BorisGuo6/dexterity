#ifndef _HALLEFFECTSENSORS_H_
#define _HALLEFFECTSENSORS_H_

#include <Arduino.h>
#include <ResponsiveAnalogRead.h>
#include <esp_now.h>
#include <WiFi.h>
#include <HapticGlove_ESPNOW.h>
#include <stdint.h>

#define SENSOR_COUNT 16

class HallEffectSensors {
    public:
        int32_t angles[SENSOR_COUNT];
        HallEffectSensors();
        HallEffectSensors(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, int32_t MCP_flexion_min,
        int32_t MCP_flexion_max, int32_t MCP_abduction_min, int32_t MCP_abduction_max, uint8_t peer_mac_addr[6]);
        void initialize();
        void updateAngles();
        void printAngles();
        void sendData();

    private:
        uint8_t s0;
        uint8_t s1;
        uint8_t s2;
        uint8_t s3;
        int32_t MCP_flexion_min;
        int32_t MCP_flexion_max;
        int32_t MCP_abduction_min;
        int32_t MCP_abduction_max;
        uint8_t peer_mac_addr[6];

        int32_t rawVals[SENSOR_COUNT];
        int32_t proto_angles[SENSOR_COUNT];
        int32_t min_angles[SENSOR_COUNT];
        int32_t max_angles[SENSOR_COUNT];
        ResponsiveAnalogRead analog(A1, true);
    
        float32_t polyVals[16][3] = {
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

        int32_t adjustMCPAbductionAngle(int_32 i);
        int32_t adjustMCPFlexionAngle(int_32 i);
        int32_t adjustPIPFlexionAngle(int_32 i);
        void measureAngles();
        void calibrate();
        void adjustAngles();
        void initializeCalibrationValues();
}

#endif