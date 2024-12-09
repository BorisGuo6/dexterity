#include "HallEffectSensors.h"
#include <Arduino.h>

#define ANGLE_LIMIT 9.0

ResponsiveAnalogRead analog(HALL_ADC, true);

int32_t rawVals[SENSOR_COUNT];
float proto_angles[SENSOR_COUNT];
float min_angles[SENSOR_COUNT];
float max_angles[SENSOR_COUNT];

float polyVals[16][3] = {
    {0.000187500000000,-0.900000000000008,1123.125000000007276}, //0 pinkie abduction
    {-0.000050824632326,0.326934971850896,-345.507279085339803}, //1 pinkie mcp flex
    {0.000044387376918,-0.286448052031499,461.919500838532997}, //2 pinkie pip flex
    {0.000187500000000,-0.900000000000008,1123.125000000007276}, //3 ring abduction 
    {-0.000050824632326,0.326934971850896,-345.507279085339803}, //4 ring mcp flex
    {0.000044387376918,-0.286448052031499,461.919500838532997}, //5 ring pip flex
    {0.000187500000000,-0.900000000000008,1123.125000000007276}, //6 middle abduction
    {-0.000050824632326,0.326934971850896,-345.507279085339803}, //7 middle mcp flex
    {0.000044387376918,-0.286448052031499,461.919500838532997}, //8 middle pip flex
    {0.000187500000000,-0.900000000000008,1123.125000000007276}, //9 index abduction
    {-0.000050824632326,0.326934971850896,-345.507279085339803},//10 index mcp flex
    {0.000044387376918,-0.286448052031499,461.919500838532997}, //11 index pip flex
    {0.000048527756939,-0.317934171042760,519.283570892722651},//12 thumb mcp flex
    {0.000187500000000,-0.900000000000008,1123.125000000007276}, //13 thumb abduction
    {0.000044387376918,-0.286448052031499,461.919500838532997}, //14 thumb pip flex
    {0.000038961038961,-0.270000000000002,459.896103896105330} //15 thumb rotation
};

const int reorder[16] = {15, 14, 13, 12, 10, 11, 9, 7, 8, 6, 4, 5, 1, 2, 0, 3};

void hallEffectSensorsSetup(){
    analogReadResolution(12);

    //pinMode(D2, OUTPUT);

    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);

    //digitalWrite(D2, LOW);

    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        min_angles[i] = 10000;
        max_angles[i] = -10000;
    }
}

float poly(double x, double a,double b,double c){
    return a*pow(x,2)+b*x+c;
}

void calibrateHallEffectSensors(){
    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        if(proto_angles[i] < min_angles[i]){
            min_angles[i] = proto_angles[i];
        } else if(proto_angles[i] > max_angles[i]){
            max_angles[i] = proto_angles[i];
        }
    }
}

void reorderArray(int32_t arr[16]) {
    int32_t temp[16]; // Temporary array to store reordered elements

    // Reorder the elements based on the order array
    for (int i = 0; i < 16; i++) {
        temp[i] = arr[reorder[i]];
    }

    // Copy the reordered elements back to the original array
    for (int i = 0; i < 16; i++) {
        arr[i] = temp[i];
    }
}

void measureHallEffectSensors()
{
    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        digitalWrite(S0, i & 0b1);
        digitalWrite(S1, (i>>1) & 0b1);
        digitalWrite(S2, (i>>2) & 0b1);
        digitalWrite(S3, (i>>3) & 0b1);

        //vTaskDelay(1 / portTICK_PERIOD_MS);
        //delay(5); //not sure if this is necessary


        analog.update();
        int32_t rawVal = analog.getRawValue();
        rawVals[i] = rawVal;
     }
    reorderArray(rawVals);

    for (uint8_t i = 0; i < SENSOR_COUNT; i++){
        // apply poly fit to get est. for angle
        float angle = poly(rawVals[i],polyVals[i][0],polyVals[i][1],polyVals[i][2]);

        // compare previous angle with current
        float angle_diff = angle - proto_angles[i];
        float angle_diff_abs = abs(angle_diff);

        if (angle_diff_abs >= ANGLE_LIMIT)
        {
            if (angle_diff >= 0.0)
            {
                proto_angles[i] = proto_angles[i] + ANGLE_LIMIT;
            }
            else
            {
                proto_angles[i] = proto_angles[i] - ANGLE_LIMIT;
            }
        }
        else
        {
            proto_angles[i] = angle;
        }  
    }
    //jank solution to having the angles for the thumb backwards
    //TODO remove with glove v2
    // proto_angles[12] = 150-proto_angles[12];
    // proto_angles[13] = 150-proto_angles[12];
    // proto_angles[14] = 150-proto_angles[14];
    // proto_angles[15] = 150-proto_angles[15];
}

