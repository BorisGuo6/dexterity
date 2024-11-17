#include "EmaxServoControl.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

bool flip_servo_map[SERVO_COUNT] = {
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0
};


void emaxServoControlSetup(){
  Wire.begin(I2C_SDA, I2C_SCL);                         
  Wire.setClock(I2C_CLOCK);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
}

void controlEmaxServo(uint8_t servo_num, uint16_t pos){
  if (flip_servo_map[servo_num]){
    pos = GLOVE_INPUT_MAX - pos;
  }
  // jank solution to converting degrees to pulse length, given that pos is between 0-200 since
  //SERVOMIN = 150 and SERVOMAX = 550
  uint16_t pulse_len = (pos * 2) + SERVOMIN;
  pwm.setPWM(servo_num, 0, pulse_len);
}




