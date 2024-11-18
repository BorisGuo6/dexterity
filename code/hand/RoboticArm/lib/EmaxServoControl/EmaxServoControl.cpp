#include "EmaxServoControl.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

bool flip_servo_map[SERVO_COUNT] = {
  0,0,0,0,
  1,1,0,0,
  1,1,0,1,
  0,1,0,0
};


void emaxServoControlSetup(){
  Wire.begin(I2C_SDA, I2C_SCL);                         
  Wire.setClock(I2C_CLOCK);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
}

void controlEmaxServo(uint8_t servo_num, uint16_t pos){
  pos = constrain(pos, GLOVE_INPUT_MIN, GLOVE_INPUT_MAX);
  uint16_t pulse_len;
  if (flip_servo_map[servo_num]){
    pulse_len = SERVOMAX - pos;
  } else{
    pulse_len = SERVOMIN + pos;
  }
  //Serial.println(pulse_len);
  pwm.setPWM(servo_num, 0, pulse_len);
}




