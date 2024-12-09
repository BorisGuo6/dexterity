#include "HapticFeedback.h"

uint8_t k;
unsigned long time0_elapsed;
TwoWire I2C_LRA= TwoWire(0);
Adafruit_DRV2605 drv[5];
uint8_t force_settings[5];
void mux_select(uint8_t channel);

void setupFeedback(){
  k = 1;
  if(TRACK_ISR_0) time0_elapsed = micros();
  I2C_LRA.begin(HAPTIC_SDA, HAPTIC_SCL, 400000);
  for(int j=0; j<5; j++){
    mux_select(j);
    drv[j].begin(&I2C_LRA);
    drv[j].selectLibrary(6);
    drv[j].setMode(DRV2605_MODE_INTTRIG);
    drv[j].useLRA();
  }
}

void triggerFeedback(){
  for(int j=0; j<5; j++){
    mux_select(j);
    switch(glove_inData.forces[j]){
      case 1:
        setHapticMode(drv[j], HAPTIC_1);
        drv[j].go();
        break;
      case 2:
        setHapticMode(drv[j], HAPTIC_2);
        drv[j].go();
        break;
      case 3:
        setHapticMode(drv[j], HAPTIC_3);
        drv[j].go();
        break;
      case 4:
        setHapticMode(drv[j], HAPTIC_4);
        drv[j].go();
        break;
      case 5:
        setHapticMode(drv[j], HAPTIC_5);
        drv[j].go();
        break;
      default:
        break;
    }
  }
  if(k % 200 == 0 && ENABLE_ESPNOW_PRINT){
    glove_monitorSuccess();
    if(!TRACK_ISR_0) k = 1;
  }
  if(k % 200 == 0 && TRACK_ISR_0){
    Serial.print("\ncallback 0 run at "); Serial.print((200*1000000)/(micros() - time0_elapsed)); Serial.println(" Hz\n");
    k = 1;
    time0_elapsed = micros();
  }
  k++;
}

void mux_select(uint8_t channel)
{
    if (channel > 7) return;

    I2C_LRA.beginTransmission(0x70);
    I2C_LRA.write(1 << channel);
    I2C_LRA.endTransmission();  
}