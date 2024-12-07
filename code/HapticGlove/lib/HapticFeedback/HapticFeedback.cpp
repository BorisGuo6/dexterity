#include "HapticFeedback.h"

int i;
TwoWire I2C_LRA= TwoWire(0);
Adafruit_DRV2605 drv;
uint8_t index_setting;

void setupFeedback(){
  I2C_LRA.begin(I2C_SDA, I2C_SCL, 400000);
  drv.begin(&I2C_LRA);
  drv.selectLibrary(6);
  drv.setMode(DRV2605_MODE_INTTRIG);
  drv.useLRA();
}

void triggerFeedback(){
  index_setting = glove_inData.force_index;
  switch(index_setting){
    case 1:
      setHapticMode(drv, HAPTIC_1);
      drv.go();
      break;
    case 2:
      setHapticMode(drv, HAPTIC_2);
      drv.go();
      break;
    case 3:
      setHapticMode(drv, HAPTIC_3);
      drv.go();
      break;
    case 4:
      setHapticMode(drv, HAPTIC_4);
      drv.go();
      break;
    case 5:
      setHapticMode(drv, HAPTIC_5);
      drv.go();
      break;
    default:
      break;
  }
  if(i % 200 == 0 && ENABLE_ESPNOW_PRINT){
    glove_monitorSuccess();
    i = 1;
  }
  i += 1;
}

void mux_select(uint8_t channel)
{
    if (channel > 7) return;

    I2C_LRA.beginTransmission(0x70);
    I2C_LRA.write(1 << channel);
    I2C_LRA.endTransmission();  
}

