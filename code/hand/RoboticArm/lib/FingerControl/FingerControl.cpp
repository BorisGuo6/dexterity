#include "FingerControl.h"

void fingerControlSetup(){
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    IM0_servo.setPeriodHertz(50); // Standard 50hz servo
    IM1_servo.setPeriodHertz(50); // Standard 50hz servo

    IM0_servo.attach(IM0_SERVO_PIN, MIN_US, MAX_US);
    IM1_servo.attach(IM1_SERVO_PIN, MIN_US, MAX_US);
}

void controlFingers(uint8_t finger_pos[]){
    
}
